#include "../../include/core/Simulation.h"

#include <iostream>

#include "config/ConfigParser.h"
#include "config/ConfigValidator.h"
#include "scheduler/EDFScheduler.h"
#include "scheduler/RMScheduler.h"


Simulation::Simulation():clock(0) { initialize(); }

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
}




void Simulation::initialize()
{
    std::string filePath;

    std::cout << "Digite o caminho do arquivo de configuração: ";
    std::cin >> filePath;

    Config config;
    try
    {
        config = ConfigParser::parse(filePath);
        ConfigValidator::validate(config);
    } catch (std::exception e)
    {
        std::cerr << "Erro ao abrir/validar arquivo de configuração: " << e.what() << '\n';
    }

    loadGlobalConfigs(config);
    loadTasks(config);
}

void Simulation::loadGlobalConfigs(const Config& config)
{
    //  Sets the scheduler
    if (config.scheduler == "RM")
        scheduler = new RMScheduler();
    else
        scheduler = new EDFScheduler();

    //  Sets the quantum
    quantum = config.quantum;

    //  Sets the CPUs
    for (int i = 0; i < config.cpuCount; i++)
        cpus.push_back(new CPU());
}

void Simulation::loadTasks(const Config& config)
{
    //  Creates each task
    for (const TaskStruct task: config.tasks)
        tasks.push_back(new Task(task.id, task.color, task.start, task.duration, task.period, task.deadline));
}




/*
    Main loop of the simulation
*/
void Simulation::start()
{
    while (!isFinished())
    {
        checkInstancesExecution();
        checkInstancesDeadlines();
        checkTasksArrivals();

        saveToHistory();

        if (shouldCallScheduler)
            callScheduler();
        executeInstances();

        tick();
        unsetCallScheduler();
    }
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
        if (cpu->isActive() || instance != nullptr)
            continue;

        //  Checking if the task is null
        Task* task = instance->getTask();
        if (task == nullptr)
            continue;

        //  Checks if it executed all its duration
        if (instance->getExecutedTime() >= task->getDuration())
        {
            instance->setTaskState(TaskState::FINISHED);

            //  Remove instance from CPU
            cpu->removeTaskInstance();
            cpu->setActive(false);

            //  Must call the scheduler after this
            setCallScheduler();
        }

        /*
        //  Checks if it runned out of quantum
        if (instance->getCurrentQuantum() >= quantum)
        {
            instance->setTaskState(TaskState::READY);

            //  Return instance to the ready queue
            readyQueue.push_back(instance);

            //  Remove instance from CPU
            cpu->removeTaskInstance();
            cpu->setActive(false);

            //  Must call the scheduler after this
            setCallScheduler();
        }
        */
    }
}

void Simulation::checkInstancesDeadlines()
{
    for (TaskInstance* instance: readyQueue)
        //  Checking if the instance missed its deadline
        if (clock > instance->getAbsoluteDeadline())
            instance->setDeadlineMissed(true);
}

void Simulation::checkTasksArrivals()
{
    for (Task* task: tasks)
    {
        //  We want to check just the tasks that didn't finished yet
        if (task->isFinished())
            continue;

        //  Checks if the task arrived
        if (clock >= task->getStart() && (clock - task->getStart()) % task->getPeriod() == 0)
        {
            task->incrementActivationCount();

            //  Creates a new instance of the task and putting it in the ready queue
            TaskInstance* instance = new TaskInstance(task, clock);
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
            continue;

        instance->incrementExecutedTime();
        //instance->incrementCurrentQuantum();
    }
}




/*
    Save the current SystemState into the history
*/
void Simulation::saveToHistory()
{

}


/*
 Advances the system clock
*/
void Simulation::tick()
{
    clock++;
}
