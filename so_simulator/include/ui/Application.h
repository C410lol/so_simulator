#ifndef SIMULADOR_ESCALONAMENTO_APPLICATION_H
#define SIMULADOR_ESCALONAMENTO_APPLICATION_H

#include "../core/Simulation.h"

#include <string>

class Application
{
private:
    Simulation simulation;
    bool initialized = false;
    bool showConfigError = false;
    bool ganttExported = false;
    std::string configPath;
    std::string configError;
    std::string stateEditMessage;
    bool stateEditFailed = false;

    bool loadConfiguration(const std::string& path);
    void renderWelcome();
    void renderSimulation();
    void renderSchedulerPanel();
    void renderReadyQueuePanel();
    void renderTasksPanel();
    void renderGanttPanel();
    void renderControls();
    void exportGanttSvg() const;

public:
    Application() = default;
    void start();
};

#endif // SIMULADOR_ESCALONAMENTO_APPLICATION_H
