#include "../../include/core/Simulation.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include "config/ConfigParser.h"
#include "config/ConfigValidator.h"
#include "scheduler/EDFScheduler.h"
#include "scheduler/RMScheduler.h"


Simulation::Simulation(std::string filePath):
clock(0), quantum(0), shouldCallScheduler(false),
schedulingWasRandom(false), prerunRunned(false), restoreNextHistoryState(false)
{
    initialize(std::move(filePath));
}
Simulation::Simulation():
clock(0), quantum(0), shouldCallScheduler(false),
schedulingWasRandom(false), prerunRunned(false), restoreNextHistoryState(false) {}

Simulation::~Simulation()
{
    //  Deletes all tasks
    for (const Task* task: tasks)
        delete task;

    //  Deletes all tasks instances
    for (const TaskInstance* instance: instances)
        delete instance;

    //  Deletes all CPUs
    for (const CPU* cpus: cpus)
        delete cpus;

    delete scheduler;
}




bool Simulation::initialize(std::string filePath)
{
    Config config;
    try
    {
        config = ConfigParser::parse(filePath);
        ConfigValidator::validate(config);
    } catch (const std::exception& e)
    {
        lastError = e.what();
        return false;
    }

    lastError.clear();
    loadGlobalConfigs(config);
    loadTasks(config);
    return true;
}

void Simulation::loadGlobalConfigs(const Config& config)
{
    //  Sets the scheduler
    if (config.scheduler == "RM")
        scheduler = new RMScheduler();
    else
        scheduler = new EDFScheduler();
    schedulerName = config.scheduler;

    //  Sets the quantum
    quantum = config.quantum;

    //  Sets the CPUs
    for (int i = 0; i < config.cpuCount; i++)
        cpus.push_back(new CPU());
}

/*
    Load tasks from config file
*/
void Simulation::loadTasks(const Config& config)
{
    //  Creates each task
    for (const TaskStruct task: config.tasks)
        tasks.push_back(new Task(task.id, task.color, task.start, task.duration, task.period, task.deadline));
}




/*
    Load tasks from system state
*/
void Simulation::loadTasks(const std::vector<TaskDTO>& _tasks)
{
    for (Task* task: tasks)
        delete task;
    tasks.clear();

    for (const TaskDTO& taskDTO: _tasks)
        tasks.push_back(new Task(taskDTO));
}

/*
    Load instances from system state
*/
void Simulation::loadInstances(const std::vector<TaskInstanceDTO>& _instances)
{
    for (TaskInstance* instance: instances)
        delete instance;
    instances.clear();

    for (TaskInstanceDTO instanceDTO: _instances)
    {
        TaskInstance* newInstance = new TaskInstance(instanceDTO);

        //  Since we stored just the task id, we must find the object itself and point to it
        newInstance->setTask(findTaskById(instanceDTO.taskId));
        newInstance->getTask()->addInstance(newInstance);

        instances.push_back(newInstance);
    }
}

/*
    Load cpus from system state
*/
void Simulation::loadCpus(const std::vector<CpuDTO>& _cpus)
{
    for (CPU* cpu: cpus)
        delete cpu;
    cpus.clear();

    for (CpuDTO cpuDTO: _cpus)
    {
        CPU* newCPU = new CPU(cpuDTO);

        //  Since we stored just the instance id, we must find the object itself and point to it
        newCPU->setTaskInstance(findInstanceById(cpuDTO.currentInstanceId));

        cpus.push_back(newCPU);
    }
}

/*
    Load ready queue by getting instances that are marked as READY in the global instances list
*/
void Simulation::loadReadyQueue()
{
    readyQueue.clear();

    for (TaskInstance* instance: instances)
        if (instance->getTaskState() == TaskState::READY)
            readyQueue.push_back(instance);
}




Task* Simulation::findTaskById(int id)
{
    for (Task* task: tasks)
        if (task->getId() == id)
            return task;
    return nullptr;
}

TaskInstance* Simulation::findInstanceById(int id)
{
    for (TaskInstance* instance: instances)
        if (instance->getId() == id)
            return instance;
    return nullptr;
}






void Simulation::prerun()
{
    if (prerunRunned)
        return;

    // Reuse the saved next snapshot while replaying an unchanged future timeline.
    if (restoreNextHistoryState && history.hasFuture())
    {
        retrieveNextFromHistory();
        restoreNextHistoryState = false;
        prerunRunned = true;
        return;
    }
    restoreNextHistoryState = false;

    checkInstancesExecution();
    checkInstancesDeadlines();
    checkTasksArrivals();

    if (shouldCallScheduler)
        callScheduler();

    saveToHistory();

    prerunRunned = true;
}

void Simulation::back()
{
    retrieveFromHistory();
}

/*
    Main loop of the simulation
*/
void Simulation::runAll()
{
    while (!isFinished())
    {
        prerun();
        run();
    }
}

void Simulation::run()
{
    executeInstances();
    tick();
    // Keep the saved future available for the next step after returning to the past.
    restoreNextHistoryState = history.hasFuture();
    unsetCallScheduler();

    prerunRunned = false;
}




bool Simulation::isFinished()
{
    //  Both tasks and instances must be finished before finishing the simulation
    if (isTasksFinished() && isInstancesFinished())
        return true;
    return false;
}

/*
    Checking if all tasks are finished
*/
bool Simulation::isTasksFinished()
{
    for (const Task* task: tasks)
        if (!task->isFinished())
            return false;
    return true;
}

/*
    Checking if all instances are finished
*/
bool Simulation::isInstancesFinished()
{
    for (const TaskInstance* instance: instances)
        if (instance->getTaskState() != TaskState::FINISHED)
            return false;
    return true;
}




void Simulation::checkInstancesExecution()
{
    for (CPU* cpu: cpus)
    {
        //  We want just the CPUs that are executing a instance
        TaskInstance* instance = cpu->getTaskInstance();
        if (!cpu->isActive() || instance == nullptr)
            continue;

        //  Checking if the task is null
        Task* task = instance->getTask();
        if (task == nullptr)
            continue;

        //  Checks if it executed all its duration
        if (instance->getExecutedTime() >= task->getDuration())
        {
            instance->setTaskState(TaskState::FINISHED);
            instance->setFinishedTime(clock);

            //  If the task of the instance reached the limit of times of activation, we set as finished
            if (instance->getTask()->activatedLimitOfTimes())
                instance->getTask()->setFinished(true);

            //  Remove instance from CPU
            cpu->removeTaskInstance();
            cpu->setActive(false);

            //  Must call the scheduler after this
            setCallScheduler();
        }
    }
}

void Simulation::checkInstancesDeadlines()
{
    for (TaskInstance* instance: instances)
        //  Checking if the instance missed its deadline
        if (
            instance->getTaskState() != TaskState::FINISHED &&
            clock >= instance->getAbsoluteDeadline()
            && !instance->isDeadlineMissed()
        )
        {
            instance->setDeadlineMissed(true);
            instance->setDeadlineMissedTime(clock);
        }
}

void Simulation::checkTasksArrivals()
{
    for (Task* task: tasks)
    {
        //  We want to check just the tasks that didn't finished or reached the limit of activation
        if (task->isFinished() || task->activatedLimitOfTimes())
            continue;

        //  Checks if the task arrived
        if (clock >= task->getStart() && (clock - task->getStart()) % task->getPeriod() == 0)
        {
            //  Creates a new instance of the task and putting it in the ready queue
            TaskInstance* instance = new TaskInstance(task, clock);

            task->addInstance(instance);
            task->incrementActivationCount();


            instances.push_back(instance);
            readyQueue.push_back(instance);

            //  Must call the scheduler after this
            setCallScheduler();
        }
    }
}




void Simulation::callScheduler()
{
    if (scheduler)
        scheduler->schedule(readyQueue, cpus, schedulingWasRandom);
}
void Simulation::setCallScheduler() { shouldCallScheduler = true; }
void Simulation::unsetCallScheduler() { shouldCallScheduler = false; }




/*
    Increasing in 1 all the tasks that are currently running
*/
void Simulation::executeInstances()
{
    for (CPU* cpu: cpus)
    {
        TaskInstance* instance = cpu->getTaskInstance();
        if (!cpu->isActive() || instance == nullptr)
        {
            //  If the CPU is not currently running a task, we increase its idle ticks
            cpu->incrementIdleTicks();
            continue;
        }

        instance->incrementExecutedTime();
        //instance->incrementCurrentQuantum();
    }
}




/*
    Save the current SystemState into the history
*/
void Simulation::saveToHistory()
{
    const SystemState currentSystemState(clock, schedulingWasRandom, schedulerName, tasks, instances, cpus);
    history.save(currentSystemState, clock);
}

/*
    Go back to the last saved system state
*/
void Simulation::retrieveFromHistory()
{
    SystemState lastSystemState = history.previous();

    clock = lastSystemState.getClock();
    schedulingWasRandom = lastSystemState.getSchedulingWasRandom();

    // Restore the scheduler implementation stored with this historical snapshot.
    const std::string historicalScheduler = lastSystemState.getSchedulerName();
    if (schedulerName != historicalScheduler)
    {
        delete scheduler;
        scheduler = historicalScheduler == "RM"
            ? static_cast<IScheduler*>(new RMScheduler())
            : static_cast<IScheduler*>(new EDFScheduler());
        schedulerName = historicalScheduler;
    }

    loadTasks(lastSystemState.getTasks());
    loadInstances(lastSystemState.getInstances());
    loadCpus(lastSystemState.getCpus());

    loadReadyQueue();
}

void Simulation::retrieveNextFromHistory()
{
    SystemState nextSystemState = history.next();

    clock = nextSystemState.getClock();
    schedulingWasRandom = nextSystemState.getSchedulingWasRandom();

    // Restore the scheduler implementation stored with this historical snapshot.
    const std::string historicalScheduler = nextSystemState.getSchedulerName();
    if (schedulerName != historicalScheduler)
    {
        delete scheduler;
        scheduler = historicalScheduler == "RM"
            ? static_cast<IScheduler*>(new RMScheduler())
            : static_cast<IScheduler*>(new EDFScheduler());
        schedulerName = historicalScheduler;
    }

    loadTasks(nextSystemState.getTasks());
    loadInstances(nextSystemState.getInstances());
    loadCpus(nextSystemState.getCpus());

    loadReadyQueue();
}




/*
 Advances the system clock
*/
void Simulation::tick() { clock++; }



//  GETERS
int Simulation::getCurrentClock() const { return clock; }
std::vector<Task*> Simulation::getTasks() const { return tasks; }
std::vector<TaskInstance*> Simulation::getInstances() const { return instances; }
std::vector<CPU*> Simulation::getCpus() const { return cpus; }
std::vector<TaskInstance*> Simulation::getReadyQueue() const { return readyQueue; }
History Simulation::getHistory() const { return history; }

void Simulation::setScheduler(const std::string& algorithm)
{
    bool changed = false;
    if (algorithm == "RM" && schedulerName != "RM")
    {
        delete scheduler;
        scheduler = new RMScheduler();
        schedulerName = "RM";
        changed = true;
    }
    else if (algorithm == "EDF" && schedulerName != "EDF")
    {
        delete scheduler;
        scheduler = new EDFScheduler();
        schedulerName = "EDF";
        changed = true;
    }
    if (changed)
    {
        callScheduler();
        // A scheduler change makes every later snapshot belong to an obsolete timeline.
        history.discardFuture();
        saveToHistory();
    }
}

bool Simulation::setInstanceState(TaskInstance* instance, TaskState state)
{
    if (instance == nullptr || state == TaskState::FINISHED ||
        instance->getTaskState() == TaskState::FINISHED ||
        (state != TaskState::READY && state != TaskState::RUNNING))
        return false;

    if (state == TaskState::RUNNING && cpus.empty())
        return false;

    const auto instanceIt = std::find(instances.begin(), instances.end(), instance);
    if (instanceIt == instances.end())
        return false;

    const TaskState previousState = instance->getTaskState();

    // Detach the instance from its current CPU or queue before assigning the new state.
    for (CPU* cpu : cpus)
    {
        if (cpu != nullptr && cpu->getTaskInstance() == instance)
        {
            cpu->removeTaskInstance();
            cpu->setActive(false);
        }
    }
    std::erase(readyQueue, instance);

    if (state == TaskState::READY)
    {
        instance->setTaskState(TaskState::READY);
        readyQueue.push_back(instance);

        // Fill the CPU released by a running instance, without selecting that
        // same instance again during this scheduling pass.
        if (previousState == TaskState::RUNNING && scheduler != nullptr)
        {
            std::vector<TaskInstance*> schedulableQueue = readyQueue;
            std::erase(schedulableQueue, instance);
            scheduler->schedule(schedulableQueue, cpus, schedulingWasRandom);
            readyQueue = std::move(schedulableQueue);
            readyQueue.push_back(instance);
        }
    }
    else
    {
        // Prefer an idle CPU. If all CPUs are busy, preempt the running
        // instance with the lowest priority under the active scheduling policy.
        CPU* selectedCpu = nullptr;
        for (CPU* cpu : cpus)
        {
            if (cpu != nullptr && (!cpu->isActive() || cpu->getTaskInstance() == nullptr))
            {
                selectedCpu = cpu;
                break;
            }
        }
        if (selectedCpu == nullptr)
        {
            std::vector<TaskInstance*> runningInstances;
            for (CPU* cpu : cpus)
                if (cpu != nullptr && cpu->isActive() && cpu->getTaskInstance() != nullptr)
                    runningInstances.push_back(cpu->getTaskInstance());

            TaskInstance* lowestPriority = scheduler != nullptr
                ? scheduler->selectLowestPriorityFrom(runningInstances, schedulingWasRandom)
                : (runningInstances.empty() ? nullptr : runningInstances.front());

            for (CPU* cpu : cpus)
                if (cpu != nullptr && cpu->getTaskInstance() == lowestPriority)
                {
                    selectedCpu = cpu;
                    break;
                }

            if (selectedCpu == nullptr)
                return false;

            TaskInstance* displaced = selectedCpu->getTaskInstance();
            if (displaced != nullptr)
            {
                displaced->setTaskState(TaskState::READY);
                if (std::find(readyQueue.begin(), readyQueue.end(), displaced) == readyQueue.end())
                    readyQueue.push_back(displaced);
            }
        }
        selectedCpu->setTaskInstance(instance);
        selectedCpu->setActive(true);
        instance->setTaskState(TaskState::RUNNING);
    }

    // A manual change starts a new history branch at the current tick.
    history.discardFuture();
    restoreNextHistoryState = false;
    saveToHistory();
    return true;
}

std::string Simulation::getSchedulerName() const { return schedulerName; }
bool Simulation::isSimulationFinished() { return isFinished(); }
bool Simulation::canGoBack() const { return history.canGoBack(); }
std::string Simulation::getLastError() const { return lastError; }


