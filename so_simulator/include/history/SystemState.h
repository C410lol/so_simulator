#ifndef SIMULADOR_ESCALONAMENTO_SYSTEMSTATE_H
#define SIMULADOR_ESCALONAMENTO_SYSTEMSTATE_H
#include <vector>

#include "DTOs/CpuDTO.h"
#include "DTOs/TaskDTO.h"
#include "DTOs/TaskInstanceDTO.h"


class SystemState
{
private:
    int clock;
    bool schedulingWasRandom;

    std::vector<TaskDTO> tasks;
    std::vector<TaskInstanceDTO> instances;
    std::vector<CpuDTO> cpus;
private:
    void copyTasks(const std::vector<Task*>& _tasks);
    void copyInstances(const std::vector<TaskInstance*>& _instances);
    void copyCpus(const std::vector<CPU*>& _cpus);
public:
    SystemState(
        int _clock,
        bool _schedulingWasRandom,
        const std::vector<Task*>& _tasks,
        const std::vector<TaskInstance*>& _instances,
        const std::vector<CPU*>& _cpus
    );

    int getClock() const;
    bool getSchedulingWasRandom() const;
    std::vector<TaskDTO> getTasks() const;
    std::vector<TaskInstanceDTO> getInstances() const;
    std::vector<CpuDTO> getCpus() const;
};


#endif //SIMULADOR_ESCALONAMENTO_SYSTEMSTATE_H
