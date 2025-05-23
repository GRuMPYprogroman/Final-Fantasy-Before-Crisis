#pragma once
#include "ISubject.h"
#include <SFML/Graphics.hpp>

class IGameState : public ISubject {
public:
    virtual ~IGameState() = default;
    virtual void update(float& deltaTime) = 0;
    virtual void render() = 0;
    virtual void handleInput(const sf::Event& event) = 0;
};