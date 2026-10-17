#include "../../include/scheduler/RMScheduler.h"


RMScheduler::RMScheduler() {}


/*
 Returns the instance with shortest period or nullptr if they are equal
*/
TaskInstance* RMScheduler::selectBestCandidate(TaskInstance* instanceA, TaskInstance* instanceB, bool& wasRandom)
{
    if (instanceA->getTask()->getPeriod() < instanceB->getTask()->getPeriod())
        return instanceA;
    if (instanceB->getTask()->getPeriod() < instanceA->getTask()->getPeriod())
        return instanceB;
    return untie(instanceA, instanceB, wasRandom);
}
