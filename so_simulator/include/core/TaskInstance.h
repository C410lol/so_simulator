#ifndef SIMULADOR_ESCALONAMENTO_TASKINSTANCE_H
#define SIMULADOR_ESCALONAMENTO_TASKINSTANCE_H
#include "Task.h"
#include "TaskState.h"


class TaskInstanceDTO;

class TaskInstance
{
private:
    int id;
    Task* task;
    int activationTime;
    int absoluteDeadline;
    int executedTime;
    int currentQuantum;
    TaskState state;
    bool deadlineMissed;
public:
    TaskInstance(Task* task, int activationTime);
    TaskInstance(const TaskInstanceDTO& instanceDTO);

    //  GETS
    int getId() const;
    Task* getTask() const;
    int getActivationTime() const;
    int getAbsoluteDeadline() const;
    int getExecutedTime() const;
    int getCurrentQuantum() const;
    TaskState getTaskState() const;
    bool isDeadlineMissed() const;

    //  SETS
    void setTask(Task* _task);
    void incrementExecutedTime();
    void incrementCurrentQuantum();
    void resetCurrentQuantum();
    void setTaskState(TaskState _state);
    void setDeadlineMissed(bool _deadlineMissed);
};


#endif //SIMULADOR_ESCALONAMENTO_TASKINSTANCE_H