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
        tasks.push_back(TaskDTO(task));
}

void SystemState::copyInstances(const std::vector<TaskInstance*>& _instances)
{
    for (TaskInstance* instance: _instances)
        instances.push_back(TaskInstanceDTO(instance));
}

void SystemState::copyCpus(const std::vector<CPU*>& _cpus)
{
    for (CPU* cpu: _cpus)
        cpus.push_back(CpuDTO(cpu));
}




int SystemState::getClock() const { return clock; }
bool SystemState::getSchedulingWasRandom() const { return schedulingWasRandom; }
std::vector<TaskDTO> SystemState::getTasks() { return tasks; }
std::vector<TaskInstanceDTO> SystemState::getInstances() { return instances; }
std::vector<CpuDTO> SystemState::getCpus() { return cpus; }
