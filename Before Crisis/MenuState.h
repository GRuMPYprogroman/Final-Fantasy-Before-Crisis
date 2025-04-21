#pragma once
#include "IGameState.h"
#include "Renderer.h"
#include "Button.h"
#include <vector>
#include <variant>

class Button;
class IObserver;
class Renderer;

class MenuState : public IGameState
{
private:
	std::list<std::shared_ptr<IObserver>> list_observer_;
	std::vector<std::shared_ptr<Button>> buttons;
	std::shared_ptr<sf::Font>font;
	std::shared_ptr <sf::Text> title;
	std::shared_ptr<IRenderService> renderService;
	bool isMainMenu = true;
	void SaveGame();
	void LoadGame();
public:
	MenuState(sf::Font&& font, std::shared_ptr<IRenderService> renderService_);
	~MenuState() override;
	void update(float& deltaTime) override;
	void render() override;
	void handleInput(const sf::Event& event) override;
	void Attach(std::shared_ptr<IObserver> observer) override;
	void Detach(std::shared_ptr<IObserver> observer) override;
	void Notify(std::any message) override;
};


