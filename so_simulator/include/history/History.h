#ifndef SIMULADOR_ESCALONAMENTO_HISTORY_H
#define SIMULADOR_ESCALONAMENTO_HISTORY_H
#include <vector>

#include "../core/SystemState.h"


class SystemState;

class History
{
private:
    std::vector<SystemState> states;
    int currentIndex;

public:
    void save(const SystemState& state);

    void next();
    void previous();

    SystemState& current();
};


#endif //SIMULADOR_ESCALONAMENTO_HISTORY_H