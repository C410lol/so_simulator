#ifndef SIMULADOR_ESCALONAMENTO_HISTORY_H
#define SIMULADOR_ESCALONAMENTO_HISTORY_H
#include <vector>
#include <stdexcept>

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
    const std::vector<SystemState>& getStates() const;
    int getCurrentIndex() const;
    bool canGoBack() const;
};


#endif //SIMULADOR_ESCALONAMENTO_HISTORY_H
