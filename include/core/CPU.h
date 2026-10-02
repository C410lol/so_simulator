#ifndef SIMULADOR_ESCALONAMENTO_CPU_H
#define SIMULADOR_ESCALONAMENTO_CPU_H
#include "TaskInstance.h"


class CPU
{
private:
    int id;
    bool active;

    TaskInstance* currentTask;

    int idleTicks;
};


#endif //SIMULADOR_ESCALONAMENTO_CPU_H