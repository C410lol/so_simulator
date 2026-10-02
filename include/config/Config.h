#ifndef SIMULADOR_ESCALONAMENTO_CONFIG_H
#define SIMULADOR_ESCALONAMENTO_CONFIG_H
#include <vector>

#include "core/Task.h"


class Config
{
    std::string schedulingAlgorithm;
    int quantum;
    int cpuCount;

    std::vector<Task> tasks;
};


#endif //SIMULADOR_ESCALONAMENTO_CONFIG_H