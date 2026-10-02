#ifndef SIMULADOR_ESCALONAMENTO_TASKINSTANCE_H
#define SIMULADOR_ESCALONAMENTO_TASKINSTANCE_H
#include "Task.h"
#include "TaskState.h"


class TaskInstance
{
private:
    int id;
    const Task* task;

    int activationTime;
    int absoluteDeadline;

    int executedTime;

    TaskState state;

    bool deadlineMissed;
};


#endif //SIMULADOR_ESCALONAMENTO_TASKINSTANCE_H