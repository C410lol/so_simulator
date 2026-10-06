#ifndef SIMULADOR_ESCALONAMENTO_CONFIG_H
#define SIMULADOR_ESCALONAMENTO_CONFIG_H
#include <vector>

#include "core/Task.h"


struct TaskStruct
{
    int id;
    std::string color;
    int start;
    int duration;
    int period;
    int deadline;
};

struct Config
{
    std::string scheduler;
    int quantum;
    int cpuCount;
    std::vector<TaskStruct> tasks;
};


#endif //SIMULADOR_ESCALONAMENTO_CONFIG_H