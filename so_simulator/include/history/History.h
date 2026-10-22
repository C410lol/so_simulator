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

    void save(const SystemState& state, int clock);
    // Remove snapshots after the current position when a new timeline is created.
    void discardFuture();
    SystemState& previous();
    SystemState& next();
    const std::vector<SystemState>& getStates() const;
    int getCurrentIndex() const;
    bool canGoBack() const;
    bool hasFuture() const;
};


#endif //SIMULADOR_ESCALONAMENTO_HISTORY_H
