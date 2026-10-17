#include "../../include/scheduler/EDFScheduler.h"


EDFScheduler::EDFScheduler() {}


/*
 Returns the instance with shortest/closest absolte deadline or nullptr if they are equal
*/
TaskInstance* EDFScheduler::selectBestCandidate(TaskInstance* instanceA, TaskInstance* instanceB, bool& wasRandom)
{
    if (instanceA->getAbsoluteDeadline() < instanceB->getAbsoluteDeadline())
        return instanceA;
    if (instanceB->getAbsoluteDeadline() < instanceA->getAbsoluteDeadline())
        return instanceB;
    return untie(instanceA, instanceB, wasRandom);
}
