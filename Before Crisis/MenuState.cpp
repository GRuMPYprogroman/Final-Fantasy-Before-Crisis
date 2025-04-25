#include "MenuState.h"
#include "SaveData.h"
#include "json.hpp"
#include <fstream>
#include <thread>
#include <chrono>

MenuState::MenuState(const sf::Font& font_, std::shared_ptr<RenderService> render_service, std::shared_ptr<AudioService> audio_service, std::shared_ptr<StateService> state_service)
	: render_service_(render_service),
	  audio_service_(audio_service),
	  state_service_(state_service)
{
	font = std::make_shared<sf::Font>(font_);

	title_font_ = std::make_unique<sf::Font>("../fonts/Spooky Light.ttf");
	title = std::make_shared<sf::Text>(*title_font_);
	title->setFillColor(sf::Color{136,8,8});

	background_texture_ = std::make_unique<sf::Texture>("../textures/mainMenu.png");
	background_ = std::make_unique<sf::Sprite>(*background_texture_);
	background_->scale(sf::Vector2f(render_service_->getRenderWindow().getSize().x / background_->getGlobalBounds().size.x,
								          render_service_->getRenderWindow().getSize().y / background_->getGlobalBounds().size.y));

	sf::Vector2f windowSize = sf::Vector2f{ render_service_->getWindowSize() };

	float winX = windowSize.x;
	float winY = windowSize.y;

	title->setOrigin(title->getLocalBounds().getCenter());
	title->setString("Before Crisis");
	title->setCharacterSize(24);
	title->setOrigin(title->getLocalBounds().getCenter());
	title->setPosition({ winX/2.f, 20});

	audio_service_->playMusic(MusicID::MenuTheme);

	float offset = 60;

	if (isMainMenu)
	{
		buttons.emplace_back(std::make_shared<Button>("Continue", *font, sf::Vector2f(winX / 2, 60),
			[&]()
			{
				audio_service_->playSound(SoundID::Click);
			}, 
			audio_service_));
		buttons.emplace_back(std::make_shared<Button>("New Game", *font, sf::Vector2f(winX / 2, buttons[0]->getBackground().getSize().y + buttons[0]->getBackground().getPosition().y),
			[&]()
			{
				audio_service_->playSound(SoundID::Click);
			}, 
			audio_service_));
		buttons.emplace_back(std::make_shared<Button>("Settings", *font, sf::Vector2f(winX / 2, buttons[1]->getBackground().getSize().y + buttons[1]->getBackground().getPosition().y),
			[&]()
			{
				audio_service_->playSound(SoundID::Click);
			}, 
			audio_service_));
		buttons.emplace_back(std::make_shared<Button>("Exit", *font, sf::Vector2f(winX / 2, buttons[2]->getBackground().getSize().y + buttons[2]->getBackground().getPosition().y),
			[&]()
			{
				audio_service_->playSound(SoundID::Click);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				audio_service_->stopMusic();
				state_service_->popState();
			}, 
			audio_service_));

		for (auto& button:buttons)
		{
			button->setPosition(sf::Vector2f(20.f, 0.f) - sf::Vector2f{0.f, 0.f} + sf::Vector2f{0.f,offset});
			offset += button->getBackgroundSize().y;

		}
	}
	else
	{
		for (auto& button : buttons)
		{
			button->setPosition(sf::Vector2f(winX / 2.f, 0.f) - sf::Vector2f{ button->getBackgroundSize().x / 2.f, 0.f } + sf::Vector2f{ 0.f,offset });
			offset += button->getBackgroundSize().y;

		}
	}
}

void MenuState::render()
{
	render_service_->getRenderWindow().draw(*background_);
	render_service_->getRenderWindow().draw(*title);
	for (auto& button : buttons)
	{
		render_service_->getRenderWindow().draw(button->getBackground());
		render_service_->getRenderWindow().draw(button->getLabel());
	}
}

void MenuState::handleInput(const sf::Event& event)
{
	for (auto& button : buttons)
		button->handleInput(event, render_service_->getRenderWindow());
}


void MenuState::update(float& deltaTime) {
	audio_service_->update(deltaTime);
	for (auto& button : buttons)
		button->update();
}

void MenuState::SaveGame() {
	SaveData data;
	nlohmann::json j = data;
	std::ofstream o("save.json");
	o << j.dump(4);
}

void MenuState::LoadGame() {
	std::ifstream i("save.json");
	if (i.is_open()) {
		nlohmann::json j;
		i >> j;
		SaveData data = j.get<SaveData>();
	}
}
