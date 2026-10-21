#include "../../include/scheduler/IScheduler.h"

#include <algorithm>

#include "core/CPU.h"
#include "core/TaskInstance.h"


/*
    Returns the best candidate from an especific vector
*/
TaskInstance* IScheduler::selectBestFrom(
    const std::vector<TaskInstance*>& candidates,
    bool& wasRandom
)
{
    if (candidates.empty())
        return nullptr;

    TaskInstance* bestCandidate = candidates.front();
    for (std::size_t i = 1; i < candidates.size(); ++i)
        bestCandidate = selectBestCandidate(bestCandidate, candidates[i], wasRandom);

    return bestCandidate;
}

/*
    Returns the lowest priority instance from an epecific vector
*/
TaskInstance* IScheduler::selectLowestPriorityFrom(
    const std::vector<TaskInstance*>& candidates,
    bool& wasRandom
)
{
    if (candidates.empty())
        return nullptr;

    TaskInstance* lowestPriority = candidates.front();
    for (std::size_t i = 1; i < candidates.size(); ++i)
    {
        TaskInstance* candidate = candidates[i];
        TaskInstance* preferred = selectBestCandidate(lowestPriority, candidate, wasRandom);
        lowestPriority = preferred == lowestPriority ? candidate : lowestPriority;
    }

    return lowestPriority;
}


void IScheduler::schedule(std::vector<TaskInstance*>& readyQueue, std::vector<CPU*>& cpus, bool& wasRandom)
{
    for (CPU* cpu: cpus)
    {
        //  If the CPU is running a task, inserts the currently running task at the begging of the ready queue
        TaskInstance* currentlyInstance = nullptr;
        if (cpu->isActive() && cpu->getTaskInstance() != nullptr)
        {
            currentlyInstance = cpu->getTaskInstance();
            readyQueue.insert(readyQueue.begin(), currentlyInstance);
            currentlyInstance->setTaskState(TaskState::READY);
        }

        if (readyQueue.empty())
        {
            cpu->removeTaskInstance();
            cpu->setActive(false);
            continue;
        }

        //  Compares istances to choose the best candidate
        //  'selectBestCandidate()' is a virtual function
        TaskInstance* bestCandidate = readyQueue.at(0);
        for (int i = 1; i < readyQueue.size(); i++)
            bestCandidate = selectBestCandidate(bestCandidate, readyQueue.at(i), wasRandom);

        std::erase(readyQueue, bestCandidate);

        cpu->setTaskInstance(bestCandidate);
        cpu->setActive(true);
        bestCandidate->setTaskState(TaskState::RUNNING);
    }
}


/*
    When the choose is tied, we untie using certain rules
*/
TaskInstance* IScheduler::untie(TaskInstance* instanceA, TaskInstance* instanceB, bool& wasRandom)
{
    //  1
    //  Chooses the instance that is currently running
    if (instanceA->getTaskState() == TaskState::RUNNING)
        return instanceA;
    if (instanceB->getTaskState() == TaskState::RUNNING)
        return instanceB;

    //  2
    //  Chooses the instance with the shortest deadline
    if (instanceA->getAbsoluteDeadline() < instanceB->getAbsoluteDeadline())
        return instanceA;
    if (instanceB->getAbsoluteDeadline() < instanceA->getAbsoluteDeadline())
        return instanceB;

    //  3
    //  Chooses the instance with the shortest arrival time
    if (instanceA->getActivationTime() < instanceB->getActivationTime())
        return instanceA;
    if (instanceB->getActivationTime() < instanceA->getActivationTime())
        return instanceB;

    //  4
    //  Chooses the instance with the shortest duration
    if (instanceA->getTask()->getDuration() < instanceB->getTask()->getDuration())
        return instanceA;
    if (instanceB->getTask()->getDuration() < instanceA->getTask()->getDuration())
        return instanceB;

    //  5
    //  Random selection
    wasRandom = true;
    if (std::rand() % 2)
        return instanceA;
    return instanceB;
}
