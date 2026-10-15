#ifndef SIMULADOR_ESCALONAMENTO_SIMULATION_H
#define SIMULADOR_ESCALONAMENTO_SIMULATION_H
#include <memory>

#include "SystemState.h"
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
    void initialize();
    void loadGlobalConfigs(const Config& config);
    void loadTasks(const Config& config);

    bool isFinished();
    bool isTasksFinished();
    bool isInstancesFinished();

    void checkInstancesDeadlines();
    void checkInstancesExecution();
    void checkTasksArrivals();

    void callScheduler();
    void setCallScheduler();
    void unsetCallScheduler();

    void executeInstances();

    void saveToHistory();

    void tick();
public:
    Simulation();
    ~Simulation();

    void start();
};


#endif //SIMULADOR_ESCALONAMENTO_SIMULATION_H