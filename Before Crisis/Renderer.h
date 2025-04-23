#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Button.h"
#include "Level.h"
#include "IObserver.h"
#include "RenderService.h"

class Button;
class Level;

class Renderer : public IObserver, public RenderService
{
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture atlas;      // Texture atlas for game world characters
    sf::VertexArray vertices; // For efficient world rendering
    bool LoadResources();   // Helper to load font and atlas
public:
    Renderer(unsigned int width, unsigned int height, const std::string& title = "Before Crisis");

    void Clear();
    void Display();
    void RenderWorld(const Level& level, const sf::Vector2f& playerPos);
    void RenderMessage(const std::string& message);
    void RenderButton(const Button& buttons);
    sf::RenderWindow& getRenderWindow() override { return window; }
    sf::Vector2u getWindowSize() const override { return window.getSize(); }

    void Update(const std::any message) override;
};

