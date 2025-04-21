#pragma once
#include "IGameState.h"
#include <vector>
#include <memory>

class StateStack
{
private:
    std::vector<std::unique_ptr<IGameState>> states;
    sf::RenderWindow& window;
public:
    StateStack(sf::RenderWindow& window);
    void pushState(std::unique_ptr<IGameState> state);
    void popState();
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
};

