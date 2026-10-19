#ifndef SO_SIMULATOR_CPUDTO_H
#define SO_SIMULATOR_CPUDTO_H
#include "core/CPU.h"


class CpuDTO
{
public:
    int id;
    bool active;
    int idleTicks;
    int currentTaskId;

    explicit CpuDTO(const CPU* cpu)
    {
        if (!cpu)
            return;

        id = cpu->getId();
        active = cpu->isActive();
        idleTicks = cpu->getIdleTicks();

        if (cpu->isActive() && cpu->getTaskInstance() != nullptr)
            currentTaskId = cpu->getTaskInstance()->getId();
    }
};

#endif //SO_SIMULATOR_CPUDTO_H
