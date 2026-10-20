#include "../../include/history/SystemState.h"


SystemState::SystemState(
    const int _clock,
    bool _schedulingWasRandom,
    const std::vector<Task*>& _tasks,
    const std::vector<TaskInstance*>& _instances,
    const std::vector<CPU*>& _cpus
):clock(_clock), schedulingWasRandom(_schedulingWasRandom)
{
    copyTasks(_tasks);
    copyInstances(_instances);
    copyCpus(_cpus);
}




void SystemState::copyTasks(const std::vector<Task*>& _tasks)
{
    for (Task* task: _tasks)
        tasks.emplace_back(task);
}

void SystemState::copyInstances(const std::vector<TaskInstance*>& _instances)
{
    for (TaskInstance* instance: _instances)
        instances.emplace_back(instance);
}

void SystemState::copyCpus(const std::vector<CPU*>& _cpus)
{
    for (CPU* cpu: _cpus)
        cpus.emplace_back(cpu);
}




int SystemState::getClock() const { return clock; }
bool SystemState::getSchedulingWasRandom() const { return schedulingWasRandom; }
std::vector<TaskDTO> SystemState::getTasks() const { return tasks; }
std::vector<TaskInstanceDTO> SystemState::getInstances() const { return instances; }
std::vector<CpuDTO> SystemState::getCpus() const { return cpus; }
