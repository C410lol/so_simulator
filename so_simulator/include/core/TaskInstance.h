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
    int instanceNumber;
    int activationTime;
    int absoluteDeadline;
    int executedTime;
    int currentQuantum;
    TaskState state;
    int finishedTime;
    bool deadlineMissed;
    int deadlineMissedTime;
public:
    TaskInstance(Task* task, int activationTime);
    TaskInstance(const TaskInstanceDTO& instanceDTO);

    //  GETS
    int getId() const;
    Task* getTask() const;
    int getInstanceNumber() const;
    int getActivationTime() const;
    int getAbsoluteDeadline() const;
    int getExecutedTime() const;
    int getCurrentQuantum() const;
    TaskState getTaskState() const;
    int getFinishedTime() const;
    bool isDeadlineMissed() const;
    int getDeadlineMissedTime() const;

    //  SETS
    void setTask(Task* _task);
    void incrementExecutedTime();
    void incrementCurrentQuantum();
    void resetCurrentQuantum();
    void setTaskState(TaskState _state);
    void setFinishedTime(int _finishedTime);
    void setDeadlineMissed(bool _deadlineMissed);
    void setDeadlineMissedTime(int _deadlineMissedTime);
};


#endif //SIMULADOR_ESCALONAMENTO_TASKINSTANCE_H