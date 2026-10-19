#ifndef SIMULADOR_ESCALONAMENTO_APPLICATION_H
#define SIMULADOR_ESCALONAMENTO_APPLICATION_H
#include "../core/Simulation.h"


class Application
{
private:
    Simulation simulation;
private:
    static void clearScreen();
    static void clearLastLine();

    static void printIntroduction(std::string& answer, std::string& filePath);
    void printTitle();
    static void printCommands();

    void render();
    void printGraphic();
    void printReadyQueue();
    void printCpus();
    void printTasks();
public:
    Application();

    void start();
};


#endif //SIMULADOR_ESCALONAMENTO_APPLICATION_H
