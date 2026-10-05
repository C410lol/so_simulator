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

    std::vector<Task> tasks;
    std::vector<TaskInstance> instances;
    std::vector<CPU> cpus;

    ReadyQueue readyQueue;

    std::unique_ptr<IScheduler> scheduler;

    int quantum;

    History history;

    bool running;
public:
    Simulation(Config config);

    void tick();
    void run();
    void pause();

    void nextStep();
    void previousStep();

    const SystemState& getSystemState() const;
};


#endif //SIMULADOR_ESCALONAMENTO_SIMULATION_H