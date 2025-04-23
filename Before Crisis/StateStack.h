#pragma once
#include "IGameState.h"
#include <vector>
#include <memory>

class IGameState;

class StateStack
{
private:
    std::vector<std::unique_ptr<IGameState>> states;
public:
    void pushState(std::unique_ptr<IGameState> state) { states.push_back(std::move(state)); }
    void popState() { states.pop_back(); }
    bool empty() const { return states.empty(); }
    const std::vector<std::unique_ptr<IGameState>>& getStates() const { return states; }
    IGameState* topState() const { return states.empty() ? nullptr : states.back().get();}
};

