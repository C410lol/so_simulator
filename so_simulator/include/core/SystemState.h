#ifndef SIMULADOR_ESCALONAMENTO_SYSTEMSTATE_H
#define SIMULADOR_ESCALONAMENTO_SYSTEMSTATE_H
#include <vector>

#include "CPU.h"
#include "ReadyQueue.h"


class SystemState
{
private:
    int clock;

    std::vector<Task> tasks;
    std::vector<TaskInstance> instances;
    std::vector<CPU> cpus;
private:
    void copyTasks(std::vector<Task*> _tasks);
    void copyInstances(std::vector<TaskInstance*> _instances);
    void copyCpus(std::vector<CPU*> _cpus);

    TaskInstance* findInstanceById(int id);
public:
    SystemState(
        int _clock,
        std::vector<Task*> _tasks,
        std::vector<TaskInstance*> _instances,
        std::vector<CPU*> _cpus
    );

    int getClock();
    std::vector<Task*> getTasks();
    std::vector<TaskInstance*> getInstances();
    std::vector<CPU*> getCpus();
};


#endif //SIMULADOR_ESCALONAMENTO_SYSTEMSTATE_H