#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class RenderService {
public:
    virtual ~RenderService() = default;
    virtual sf::RenderWindow& getRenderWindow() = 0;
    virtual sf::Vector2u getWindowSize() const = 0;
	virtual sf::Font getDefaultFont() = 0;
};