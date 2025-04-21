#include "MenuState.h"
#include "SaveData.h"
#include "json.hpp"
#include <fstream>

MenuState::MenuState(sf::Font&& font_, std::shared_ptr<IRenderService> renderService_)
	: renderService(renderService_)
{
	font = std::make_shared<sf::Font>(font_);
	title = std::make_shared<sf::Text>(*font);

	list_observer_.push_back(std::dynamic_pointer_cast<IObserver>(renderService));
	sf::Vector2f windowSize = sf::Vector2f{ renderService->getWindowSize() };

	float winX = windowSize.x;
	float winY = windowSize.y;

	title->setOrigin(title->getLocalBounds().getCenter());
	title->setString("Before Crisis");
	title->setCharacterSize(24);
	title->setOrigin(title->getLocalBounds().getCenter());
	title->setPosition({ winX/2.f, 20});

	float offset = 60;

	if (isMainMenu)
	{
		buttons.emplace_back(std::make_shared<Button>("Continue", *font, sf::Vector2f(winX / 2, 60),
			[&]()
			{

			}));
		buttons.emplace_back(std::make_shared<Button>("New Game", *font, sf::Vector2f(winX / 2, buttons[0]->getBackground().getSize().y + buttons[0]->getBackground().getPosition().y),
			[&]()
			{
				
			}));
		buttons.emplace_back(std::make_shared<Button>("Settings", *font, sf::Vector2f(winX / 2, buttons[1]->getBackground().getSize().y + buttons[1]->getBackground().getPosition().y),
			[&]()
			{
				
			}));
		buttons.emplace_back(std::make_shared<Button>("Exit", *font, sf::Vector2f(winX / 2, buttons[2]->getBackground().getSize().y + buttons[2]->getBackground().getPosition().y),
			[&]()
			{
				
			}));

		for (auto& button:buttons)
		{
			button->setPosition(sf::Vector2f(winX / 2.f, 0.f) - sf::Vector2f{ button->getBackgroundSize().x / 2.f, 0.f} + sf::Vector2f{0.f,offset});
			offset += button->getBackgroundSize().y;

		}
	}
}

MenuState::~MenuState()
{
	for (auto& observer: list_observer_)
	{
		list_observer_.remove(observer);
	}
}

void MenuState::Attach(std::shared_ptr<IObserver> observer)
{
	list_observer_.push_back(observer);
}

void MenuState::Detach(std::shared_ptr<IObserver> observer)
{
	list_observer_.remove(observer);
}

void MenuState::Notify(std::any message)
{
	std::list<std::shared_ptr<IObserver>>::iterator iterator = list_observer_.begin();
	while (iterator != list_observer_.end()) {
		(*iterator)->Update(message);
		++iterator;
	}
}

void MenuState::render()
{
	Notify(title);
	for (auto& button : buttons)
		Notify(button);
}

void MenuState::handleInput(const sf::Event& event)
{
	for (auto& button : buttons)
		button->handleInput(event, renderService->getRenderWindow());
}


void MenuState::update(float& deltaTime) {
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
