#include "../../include/scheduler/IScheduler.h"

#include <algorithm>

#include "core/CPU.h"
#include "core/TaskInstance.h"


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

        readyQueue.erase(std::remove(readyQueue.begin(), readyQueue.end(), bestCandidate), readyQueue.end());

        //  If the running task was not selected as the best candidade, we must change its state
        if (currentlyInstance != nullptr && currentlyInstance->getId() != bestCandidate->getId())
            currentlyInstance->setTaskState(TaskState::READY);

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
