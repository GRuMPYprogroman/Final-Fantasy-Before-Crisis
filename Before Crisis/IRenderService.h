#pragma once

#include <SFML/Graphics.hpp>

class IRenderService {
public:
    virtual ~IRenderService() = default;
    virtual sf::RenderWindow& getRenderWindow() = 0;
    virtual sf::Vector2u getWindowSize() const = 0;
};