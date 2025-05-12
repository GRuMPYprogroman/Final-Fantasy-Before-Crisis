#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "RenderService.h"

class Renderer : public RenderService
{
private:
    sf::RenderWindow window;
    sf::Font default_font_;
public:
    Renderer(unsigned int width, unsigned int height, const std::string& title = "Before Crisis");

    void Clear();
    void Display();
    void RenderMessage(const std::string& message);
    bool LoadResources();
	sf::Font getDefaultFont() override { return default_font_; }
    sf::RenderWindow& getRenderWindow() override { return window; }
    sf::Vector2u getWindowSize() const override { return window.getSize(); }
};

