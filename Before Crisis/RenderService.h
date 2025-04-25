#pragma once

#include <SFML/Graphics.hpp>

class RenderService {
public:
    virtual ~RenderService() = default;
    virtual sf::RenderWindow& getRenderWindow() = 0;
    virtual sf::Vector2u getWindowSize() const = 0;
};