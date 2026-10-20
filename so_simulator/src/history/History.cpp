#include "../../include/history/History.h"


History::History():currentIndex(0) {}




void History::save(const SystemState& state, int clock)
{
    if (static_cast<int>(states.size()) == clock)
        states.push_back(state);
    else
        states.at(clock) = state;
    currentIndex = clock;
}

SystemState& History::previous()
{
    if (states.empty())
        throw std::out_of_range("Ainda não há estados para voltar.");
    if (currentIndex > 0)
    {
        --currentIndex;
        states.erase(states.begin() + currentIndex + 1, states.end());
    }
    return states.at(currentIndex);
}

const std::vector<SystemState>& History::getStates() const { return states; }
int History::getCurrentIndex() const { return currentIndex; }
bool History::canGoBack() const { return currentIndex > 0; }
