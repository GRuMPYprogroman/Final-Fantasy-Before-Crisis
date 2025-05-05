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
	bool isChosen = false;
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
	void setBackgroundSize(sf::Vector2f size) { background->setSize(size); label->setPosition(background->getPosition() + background->getSize() / 2.f);}

	void setChosen()
	{
		if (isChosen) isChosen = false;
		else isChosen = true;
	}

	void setCallback(std::function<void()> callback)
	{
		onClick = callback;
	}

	void setHovered(bool state) { isHovered = state; }
};
