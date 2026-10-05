#ifndef SIMULADOR_ESCALONAMENTO_CPU_H
#define SIMULADOR_ESCALONAMENTO_CPU_H
#include "TaskInstance.h"


class CPU
{
private:
    int id;
    bool active;
    int idleTicks;
    TaskInstance* currentTask;
public:
    CPU();

    //  GETS
    int getId() const;
    bool isActive() const;
    int getIdleTicks() const;
    TaskInstance* getTaskInstance() const;

    //  SETS
    void setActive(bool _active);
    void incrementIdleTicks();
    void setTaskInstance(TaskInstance* _taskInstance);
    void removeTaskInstance();
};


#endif //SIMULADOR_ESCALONAMENTO_CPU_H