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
public:
    TaskInstance(Task& task, int activationTime);

    //  GETS
    int getId() const;
    Task* getTask() const;
    int getActivationTime() const;
    int getAbsoluteDeadline() const;
    int getExecutedTime() const;
    TaskState getTaskState() const;
    bool isDeadlineMissed() const;

    //  SETS
    void incrementExecutedTime();
    void setTaskState(TaskState taskState);
    void setDeadlineMissed(bool deadlineMissed);
};


#endif //SIMULADOR_ESCALONAMENTO_TASKINSTANCE_H