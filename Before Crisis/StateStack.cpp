#include "StateStack.h"

StateStack::StateStack(sf::RenderWindow& window) : window(window) {}
void StateStack::pushState(std::unique_ptr<IGameState> state) { states.push_back(std::move(state)); }
void StateStack::popState() { if (!states.empty()) states.pop_back(); }
void StateStack::update(float deltaTime) { if (!states.empty()) states.back()->update(deltaTime); }
//void StateStack::render(sf::RenderWindow& window) { if (!states.empty()) states.back()->render(window); }