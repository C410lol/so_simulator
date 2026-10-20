#ifndef SIMULADOR_ESCALONAMENTO_CONFIGPARSER_H
#define SIMULADOR_ESCALONAMENTO_CONFIGPARSER_H
#include <algorithm>
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

        // The first value names the scheduler; normalize it before validating.
        std::getline(ss, scheduler, ';');
        std::transform(
            scheduler.begin(),
            scheduler.end(),
            scheduler.begin(),
            [](const unsigned char c) {
                return std::toupper(c);
            }
        );  //  Uppercasing all the string
        std::getline(ss, quantum, ';');
        std::getline(ss, cpus, ';');

        config.scheduler = scheduler;
        config.quantum = std::stoi(quantum);
        config.cpuCount = std::stoi(cpus);
    }

    static TaskStruct extractTaskConfigs(const std::string& taskConfigLine)
    {
        std::stringstream ss(taskConfigLine);

        std::string id;
        std::string color;
        std::string start;
        std::string duration;
        std::string period;
        std::string deadline;

        //  Extracting fields
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
            throw std::runtime_error("Erro ao abrir o arquivo.");

        std::string line;
        Config config;

        //  Extract global configs
        if (!std::getline(file, line))
            throw std::runtime_error("Primeira linha do arquivo está vazia.");
        extractGlobalConfigs(config, line);

        //  Extract tasks
        while (std::getline(file, line))
        {
            if (line.empty() || line.find_first_not_of(" \t\r") == std::string::npos)
                continue;
            if (line.back() == '\r')
                line.pop_back();
            config.tasks.push_back(extractTaskConfigs(line));
        }

        return config;
    }
};


#endif //SIMULADOR_ESCALONAMENTO_CONFIGPARSER_H
