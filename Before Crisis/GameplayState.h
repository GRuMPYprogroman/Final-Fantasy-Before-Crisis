#pragma once
#include "IGameState.h"
#include "Button.h"
#include "AudioService.h"
#include "StateService.h"
#include "RenderService.h"
#include "IGameplayPanel.h"
#include "map"
#include <functional>
#include "CharacterManager.h"

using PanelFactory = std::function<std::unique_ptr<IGameplayPanel>()>;

class GameplayState : public IGameState {
private:
    std::map<std::string, PanelFactory> panelFactories_;
    std::unique_ptr<IGameplayPanel> activePanel_;
    std::vector<std::shared_ptr<Button>> navButtons_;
    std::shared_ptr<RenderService> render_service_;
    std::shared_ptr<AudioService> audio_service_;
    std::shared_ptr<StateService> state_service_;
	std::shared_ptr<Character> player_;
	std::shared_ptr<sf::Font> font_;

    void registerPanels();
    void switchTo(const std::string& panelKey);
    void setupNavButtons();
public:
    GameplayState(std::shared_ptr<RenderService> renderService,
        std::shared_ptr<AudioService> audioService,
        std::shared_ptr<StateService> stateService,
        std::shared_ptr<Character> player);
    void update(float& dt) override;
    void render() override;
    void handleInput(const sf::Event& evt) override;
};