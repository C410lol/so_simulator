#ifndef SIMULADOR_ESCALONAMENTO_SCHEDULERFACTORY_H
#define SIMULADOR_ESCALONAMENTO_SCHEDULERFACTORY_H
#include <memory>

#include "IScheduler.h"


class SchedulerFactory
{
public:
    static std::unique_ptr<IScheduler> create(
        const std::string& algorithm
    );
};


#endif //SIMULADOR_ESCALONAMENTO_SCHEDULERFACTORY_H