#ifndef SIMULADOR_ESCALONAMENTO_SYSTEMSTATE_H
#define SIMULADOR_ESCALONAMENTO_SYSTEMSTATE_H
#include <vector>

#include "CPU.h"
#include "ReadyQueue.h"


class SystemState
{
private:
    int clock;

    std::vector<CPU> cpus;
    std::vector<TaskInstance> tasks;

    std::vector<int> readyTasksIds;
};


#endif //SIMULADOR_ESCALONAMENTO_SYSTEMSTATE_H