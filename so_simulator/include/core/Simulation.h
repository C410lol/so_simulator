#ifndef SIMULADOR_ESCALONAMENTO_SIMULATION_H
#define SIMULADOR_ESCALONAMENTO_SIMULATION_H

#include "Task.h"
#include "TaskInstance.h"
#include "CPU.h"
#include "config/Config.h"
#include "history/History.h"
#include "scheduler/IScheduler.h"


class Simulation
{
private:
    int clock;
    int quantum;

    bool shouldCallScheduler;
    bool schedulingWasRandom;

    std::vector<Task*> tasks;
    std::vector<TaskInstance*> instances;
    std::vector<CPU*> cpus;

    std::vector<TaskInstance*> readyQueue;
    IScheduler* scheduler;

    History history;
private:
    void initialize(std::string filePath);

    void loadGlobalConfigs(const Config& config);
    void loadTasks(const Config& config);
    void loadTasks(SystemState& systemState);
    void loadInstances(SystemState& systemState);
    void loadCpus(SystemState& systemState);

    Task* findTaskById(int id);
    TaskInstance* findInstanceById(int id);

    bool isFinished();
    bool isTasksFinished();
    bool isInstancesFinished();

    void checkInstancesDeadlines();
    void checkInstancesExecution();
    void checkTasksArrivals();

    void callScheduler();
    void setCallScheduler();
    void unsetCallScheduler();

    void execute();
    void executeInstances();

    void saveToHistory();
    void retrieveFromHistory();

    void tick();
public:
    Simulation(std::string filePath);
    ~Simulation();

    void start();
};


#endif //SIMULADOR_ESCALONAMENTO_SIMULATION_H