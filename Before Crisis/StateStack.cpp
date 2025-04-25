#include "StateStack.h"

void StateStack::pushState(std::unique_ptr<IGameState> state)  { states.push_back(std::move(state)); }
void StateStack::popState()  { states.pop_back(); }
bool StateStack::empty() const { return states.empty(); }
const std::vector<std::unique_ptr<IGameState>>& StateStack::getStates() const { return states; }
IGameState* StateStack::topState() const { return states.empty() ? nullptr : states.back().get(); }