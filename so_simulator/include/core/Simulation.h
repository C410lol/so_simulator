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
    IScheduler* scheduler{};

    History history;
    std::string schedulerName;
    std::string lastError;
private:
    void loadGlobalConfigs(const Config& config);
    void loadTasks(const Config& config);

    void loadTasks(const std::vector<TaskDTO>& _tasks);
    void loadInstances(const std::vector<TaskInstanceDTO>& _instances);
    void loadCpus(const std::vector<CpuDTO>& _cpus);
    void loadReadyQueue();

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

    void executeInstances();

    void saveToHistory();
    void retrieveFromHistory();

    void tick();
public:
    Simulation(std::string filePath);
    Simulation();
    ~Simulation();

    bool initialize(std::string filePath);
    void setScheduler(const std::string& algorithm);
    std::string getSchedulerName() const;
    bool isSimulationFinished();
    bool canGoBack() const;
    std::string getLastError() const;
    bool setInstanceState(int instanceId, TaskState state, std::string& error);

    void prerun();
    void back();
    void runAll();
    void run();

    int getCurrentClock() const;

    std::vector<Task*> getTasks() const;
    std::vector<TaskInstance*> getInstances() const;
    std::vector<CPU*> getCpus() const;
    std::vector<TaskInstance*> getReadyQueue() const;

    History getHistory() const;
};


#endif //SIMULADOR_ESCALONAMENTO_SIMULATION_H
