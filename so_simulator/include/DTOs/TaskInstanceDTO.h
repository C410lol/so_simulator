#ifndef SO_SIMULATOR_TASKINSTANCEDTO_H
#define SO_SIMULATOR_TASKINSTANCEDTO_H
#include "core/TaskInstance.h"
#include "core/TaskState.h"


class TaskInstanceDTO
{
public:
    int id;
    int taskId;
    int instanceNumber;
    int activationTime;
    int absoluteDeadline;
    int executedTime;
    int currentQuantum;
    TaskState state;
    int finishedTime;
    bool deadlineMissed;
    int deadlineMissedTime;

    explicit TaskInstanceDTO(const TaskInstance* instance)
    {
        if (!instance)
            return;

        id = instance->getId();
        taskId = instance->getTask()->getId();
        instanceNumber = instance->getInstanceNumber();
        activationTime = instance->getActivationTime();
        absoluteDeadline = instance->getAbsoluteDeadline();
        executedTime = instance->getExecutedTime();
        currentQuantum = instance->getCurrentQuantum();
        state = instance->getTaskState();
        finishedTime = instance->getFinishedTime();
        deadlineMissed = instance->isDeadlineMissed();
        deadlineMissedTime = instance->getDeadlineMissedTime();
    }
};

#endif //SO_SIMULATOR_TASKINSTANCEDTO_H
