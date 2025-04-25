#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <string>
#include "AudioService.h"
#include "AudioVariables.h"

class Button {
private:
	std::unique_ptr<sf::RectangleShape> background;
	std::unique_ptr<sf::Text> label;
	std::function<void()> onClick;
	std::shared_ptr<AudioService> audio_service_;
	bool isHovered;
public:
	static sf::SoundBuffer soundBuffer;
	static sf::Sound hoverSound;

	Button(const std::string& text, sf::Font& font, sf::Vector2f position, std::function<void()> onClick, std::shared_ptr<AudioService> audio_service);
	void update();
	void handleInput(const sf::Event& event,sf::RenderWindow& window);

	sf::RectangleShape getBackground() const { return *background; }
	sf::Text getLabel() const { return *label; }

	void setPosition(sf::Vector2f pos);
	sf::Vector2f getBackgroundSize() const { return background->getSize(); }
};
