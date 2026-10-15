#include "../../include/core/SystemState.h"


SystemState::SystemState(
    int _clock,
    std::vector<Task*> _tasks,
    std::vector<TaskInstance*> _instances,
    std::vector<CPU*> _cpus
):clock(_clock)
{
    copyTasks(_tasks);
    copyInstances(_instances);
    copyCpus(_cpus);
}


void SystemState::copyTasks(std::vector<Task*> _tasks)
{
    for (Task* task: _tasks)
    {
        tasks.push_back(Task(task));
    }
}

void SystemState::copyInstances(std::vector<TaskInstance*> _instances)
{
    for (TaskInstance* instance: _instances)
        instances.push_back(TaskInstance(instance));
}

void SystemState::copyCpus(std::vector<CPU*> _cpus)
{
    for (CPU* cpu: _cpus)
    {
        CPU savedCPU(cpu);
        cpus.push_back(savedCPU);

        if (savedCPU.isActive())
        {
            TaskInstance* runningInstance = findInstanceById(savedCPU.getTaskInstance()->getId());
            savedCPU.setTaskInstance(runningInstance);
        }
    }
}
