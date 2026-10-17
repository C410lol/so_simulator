#ifndef SIMULADOR_ESCALONAMENTO_HISTORY_H
#define SIMULADOR_ESCALONAMENTO_HISTORY_H
#include <vector>

#include "SystemState.h"


class SystemState;

class History
{
private:
    std::vector<SystemState> states;
    int currentIndex;
public:
    History();

    void save(const SystemState& state);
    SystemState& previous();
};


#endif //SIMULADOR_ESCALONAMENTO_HISTORY_H