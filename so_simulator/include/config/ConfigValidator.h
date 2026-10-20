#ifndef SIMULADOR_ESCALONAMENTO_CONFIGVALIDATOR_H
#define SIMULADOR_ESCALONAMENTO_CONFIGVALIDATOR_H
#include <iostream>
#include <stdexcept>
#include <string>

#include "Config.h"


class ConfigValidator
{
private:
    static void validateGlobalConfigs(const Config& config)
    {
        if (config.scheduler != "RM" && config.scheduler != "EDF")
            throw std::runtime_error("Algoritimo de escalonamento não reconhecido ou não suportado: "
                + config.scheduler);
        if (config.quantum <= 0)
            throw std::runtime_error("Valor para quantum não permitido: " + std::to_string(config.quantum));
        if (config.cpuCount <= 0)
            throw std::runtime_error("Valor para CPUs não permitido: " + std::to_string(config.cpuCount));
    }

    static void validateTasksConfigs(const Config& config)
    {
        for (const auto& task: config.tasks)
            validateTaskConfigs(task);
    }
    static void validateTaskConfigs(const TaskStruct& task)
    {
        if (task.id < 0)
            throw std::runtime_error("Valor para 'id' não permitido: " + std::to_string(task.id));
        if (task.start < 0)
            throw std::runtime_error("Valor de chegada não permitido: " + std::to_string(task.start));
        if (task.duration <= 0)
            throw std::runtime_error("Valor de duração não permitido: " + std::to_string(task.duration));
        if (task.period <= 0)
            throw std::runtime_error("Valor de período não permitido: " + std::to_string(task.period));
        if (task.deadline <= 0)
            throw std::runtime_error("Valor de prazo não permitido: " + std::to_string(task.deadline));
    }
public:
    static void validate(const Config& config)
    {
        validateGlobalConfigs(config);
        validateTasksConfigs(config);
    }
};


#endif //SIMULADOR_ESCALONAMENTO_CONFIGVALIDATOR_H
