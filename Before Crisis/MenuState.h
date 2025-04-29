#pragma once
#include "IGameState.h"
#include "AudioService.h"
#include "StateService.h"
#include "RenderService.h"
#include "Button.h"
#include <vector>

class MenuState : public IGameState
{
private:
	std::vector<std::shared_ptr<Button>> buttons;
	std::shared_ptr<sf::Font>font;
	std::unique_ptr<sf::Font> title_font_;
	std::shared_ptr <sf::Text> title;
	std::shared_ptr<RenderService> render_service_;
	std::shared_ptr<AudioService> audio_service_;
	std::shared_ptr<StateService> state_service_;
	std::unique_ptr<sf::Texture> background_texture_;
	std::unique_ptr<sf::Sprite> background_;
	bool isMainMenu = true;
	void SaveGame();
	void LoadGame();
public:
	MenuState(std::shared_ptr<RenderService> render_service, std::shared_ptr<AudioService> audio_service, std::shared_ptr<StateService> state_service);
	~MenuState() override = default;
	void update(float& deltaTime) override;
	void render() override;
	void handleInput(const sf::Event& event) override;
};


