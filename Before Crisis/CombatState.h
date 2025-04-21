#pragma once
#include "IGameState.h"

class CombatState : public IGameState
{
public:
     void update(float& deltaTime) override;
     void render() override;
     void handleInput(const sf::Event& event) override;
};

