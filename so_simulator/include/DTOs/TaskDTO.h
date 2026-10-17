#ifndef SO_SIMULATOR_TASKDTO_H
#define SO_SIMULATOR_TASKDTO_H
#include <string>

#include "core/Task.h"


class TaskDTO
{
public:
    int id;
    std::string color;
    int start;
    int duration;
    int period;
    int deadline;
    int activationCount;
    bool finished;

    explicit TaskDTO(const Task* task)
    {
        if (!task)
            return;

        id = task->getId();
        color.copy(task->getColor().data(), task->getColor().size(), 0);
        start = task->getStart();
        duration = task->getDuration();
        period = task->getPeriod();
        deadline = task->getDeadline();
        activationCount = task->getActivationCount();
        finished = task->isFinished();
    }
};

#endif //SO_SIMULATOR_TASKDTO_H
