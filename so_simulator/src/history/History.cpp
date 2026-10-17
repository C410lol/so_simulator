#include "../../include/history/History.h"


History::History():currentIndex(0) {}




void History::save(const SystemState& state)
{
    states.push_back(state);
    currentIndex++;
}

SystemState& History::previous()
{
    if (currentIndex == 0)
        return states.at(currentIndex);
    return states.at(--currentIndex);
}
