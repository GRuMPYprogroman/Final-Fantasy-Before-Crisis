#pragma once
#include "IGameState.h"

class GameplayState : public IGameState
{
public:
    void update(float& deltaTime) override;
   //void render(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event) override;
};
