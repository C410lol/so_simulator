#ifndef SIMULADOR_ESCALONAMENTO_CONFIGVALIDATOR_H
#define SIMULADOR_ESCALONAMENTO_CONFIGVALIDATOR_H
#include <iostream>
#include <stdexcept>

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
            throw std::runtime_error("Valor para quantum não permitido: " + config.quantum);
        if (config.cpuCount <= 0)
            throw std::runtime_error("Valor para CPUs não permitido: " + config.cpuCount);
    }

    static void validateTasksConfigs(const Config& config)
    {
        for (const auto& task: config.tasks)
            validateTaskConfigs(task);
    }
    static void validateTaskConfigs(const TaskStruct& task)
    {
        if (task.id < 0)
            throw std::runtime_error("Valor para 'id' não permitido: " + task.id);
        if (task.start < 0)
            throw std::runtime_error("Valor de chegada não permitido: " + task.start);
        if (task.duration <= 0)
            throw std::runtime_error("Valor de duração não permitido: " + task.duration);
        if (task.period <= 0)
            throw std::runtime_error("Valor de período não permitido: " + task.period);
        if (task.deadline <= 0)
            throw std::runtime_error("Valor de prazo não permitido: " + task.deadline);
    }
public:
    static void validate(const Config& config)
    {
        validateGlobalConfigs(config);
        validateTasksConfigs(config);
    }
};


#endif //SIMULADOR_ESCALONAMENTO_CONFIGVALIDATOR_H