#ifndef SIMULADOR_ESCALONAMENTO_CPU_H
#define SIMULADOR_ESCALONAMENTO_CPU_H
#include "TaskInstance.h"


class CPU
{
private:
    int id;
    bool active;
    int idleTicks;
    int quantum;
    TaskInstance* currentTask;
    int taskCurrentQuantum;
public:
    CPU(int quantum);

    //  GETS
    int getId() const;
    bool isActive() const;
    int getIdleTicks() const;
    TaskInstance* getTaskInstance() const;
    int getTaskCurrentQuantum() const;

    //  SETS
    void setActive(bool active);
    void incrementIdleTicks();
    void setTaskInstance(TaskInstance& taskInstance);
    void removeTaskInstance();
    void incrementTaskCurrentQuantum();
    void resetTaskCurrentQuantum();
};


#endif //SIMULADOR_ESCALONAMENTO_CPU_H