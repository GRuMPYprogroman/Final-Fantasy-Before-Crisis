#pragma once
#include <SFML/Graphics.hpp>
#include "RenderService.h"

class IGameplayPanel {
public:
    virtual ~IGameplayPanel() = default;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void handleInput(const sf::Event& event) = 0;
};
