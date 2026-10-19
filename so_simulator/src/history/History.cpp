#include "../../include/history/History.h"


History::History():currentIndex(-1) {}




void History::save(const SystemState& state)
{
    states.erase(states.begin() + currentIndex + 1, states.end());
    states.push_back(state);
    currentIndex = static_cast<int>(states.size()) - 1;
}

SystemState& History::previous()
{
    if (states.empty())
        throw std::out_of_range("Ainda não há estados para voltar.");
    if (currentIndex > 0)
        --currentIndex;
    return states.at(currentIndex);
}

const std::vector<SystemState>& History::getStates() const { return states; }
int History::getCurrentIndex() const { return currentIndex; }
bool History::canGoBack() const { return currentIndex > 0; }
