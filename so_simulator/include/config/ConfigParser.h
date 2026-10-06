#ifndef SIMULADOR_ESCALONAMENTO_CONFIGPARSER_H
#define SIMULADOR_ESCALONAMENTO_CONFIGPARSER_H
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Config.h"


class ConfigParser
{
private:
    static void extractGlobalConfigs(Config& config, const std::string& globalConfigLine)
    {
        std::stringstream ss(globalConfigLine);

        std::string scheduler;
        std::string quantum;
        std::string cpus;

        std::getline(ss, scheduler, ';');
        std::getline(ss, quantum, ';');
        std::getline(ss, cpus, ';');

        config.scheduler = scheduler;
        config.quantum = std::stoi(quantum);
        config.cpuCount = std::stoi(cpus);
    }

    static TaskStruct extracTaskConfigs(const std::string& taskConfigLine)
    {
        std::stringstream ss(taskConfigLine);

        std::string id;
        std::string color;
        std::string start;
        std::string duration;
        std::string period;
        std::string deadline;

        std::getline(ss, id, ';');
        std::getline(ss, color, ';');
        std::getline(ss, start, ';');
        std::getline(ss, duration, ';');
        std::getline(ss, period, ';');
        std::getline(ss, deadline, ';');

        TaskStruct task;
        task.id = std::stoi(id);
        task.color = color;
        task.start = std::stoi(start);
        task.duration = std::stoi(duration);
        task.period = std::stoi(period);
        task.deadline = std::stoi(deadline);

        return task;
    }
public:
    static Config parse(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file)
            std::cerr << "Erro ao abrir arquivo.\n";

        std::string line;
        Config config;

        //  Extract global configs
        std::getline(file, line);
        extractGlobalConfigs(config, line);

        //  Extract tasks
        while (std::getline(file, line))
            config.tasks.push_back(extracTaskConfigs(line));

        return config;
    }
};


#endif //SIMULADOR_ESCALONAMENTO_CONFIGPARSER_H