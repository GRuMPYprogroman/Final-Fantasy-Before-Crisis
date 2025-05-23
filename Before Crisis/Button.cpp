#include "Button.h"

sf::SoundBuffer Button::soundBuffer = sf::SoundBuffer("../audio/Menu Select.wav");
sf::Sound Button::hoverSound = sf::Sound(Button::soundBuffer);

Button::Button(const std::string& text, sf::Font& font, sf::Vector2f position, std::function<void()> onClick)
    : onClick(onClick), isHovered(false){
    background = std::make_unique<sf::RectangleShape>();
    background->setSize(sf::Vector2f(120, 50));
    background->setPosition(position);
    background->setFillColor(sf::Color(50, 50, 50));
    background->setOutlineColor(sf::Color::White);
    background->setOutlineThickness(2.f);

    label = std::make_unique<sf::Text>(font, text);
    label->setString(text);
    label->setCharacterSize(20);
    label->setFillColor(sf::Color::White);
    sf::FloatRect textBounds = label->getLocalBounds();
    label->setOrigin(textBounds.getCenter());
    label->setPosition(position + background->getSize() / 2.f);
}

void Button::update() {
    if (isHovered) background->setFillColor(sf::Color::Cyan);
    else background->setFillColor(sf::Color(50, 50, 50));
}


void Button::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (event.is<sf::Event::MouseMoved>()) {
        bool lastHovered = isHovered;
        isHovered = background->getGlobalBounds().contains(sf::Vector2f{ sf::Mouse::getPosition(window) });
        if (lastHovered != isHovered && lastHovered == false)
            hoverSound.play();
    }
    if (event.is<sf::Event::MouseButtonPressed>() && isHovered) {
        onClick();
    }
}

void Button::setPosition(sf::Vector2f pos)
{
    background->setPosition(pos);
    label->setPosition(pos + background->getSize() / 2.f);
}
