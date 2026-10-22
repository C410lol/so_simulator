#include "../../include/history/History.h"


History::History():currentIndex(0) {}




void History::save(const SystemState& state, int clock)
{
    if (clock >= static_cast<int>(states.size()))
        states.push_back(state);
    else
        states.at(clock) = state;
    currentIndex = clock;
}

void History::discardFuture()
{
    const int firstFutureIndex = currentIndex + 1;
    if (firstFutureIndex < static_cast<int>(states.size()))
        states.erase(states.begin() + firstFutureIndex, states.end());
}

SystemState& History::previous()
{
    if (states.empty())
        throw std::out_of_range("Ainda não há estados para voltar.");
    if (currentIndex > 0)
        --currentIndex;
    return states.at(currentIndex);
}

SystemState& History::next()
{
    if (!hasFuture())
        throw std::out_of_range("Ainda não há estados futuros para avançar.");
    return states.at(++currentIndex);
}

const std::vector<SystemState>& History::getStates() const { return states; }
int History::getCurrentIndex() const { return currentIndex; }
bool History::canGoBack() const { return currentIndex > 0; }
bool History::hasFuture() const { return currentIndex + 1 < static_cast<int>(states.size()); }
