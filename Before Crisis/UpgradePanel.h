#pragma once
#include "IGameplayPanel.h"
#include "RenderService.h"
#include "AudioService.h"
#include "CharacterManager.h"
#include "Button.h"
#include <vector>

class UpgradePanel : public IGameplayPanel {
private:
    std::shared_ptr<RenderService> render_service_;
    std::shared_ptr<AudioService>  audio_service_;
    std::shared_ptr<Character>     player_;

    std::shared_ptr<sf::Font>     font_;
    std::unique_ptr<sf::Text>     title_;
    std::unique_ptr<sf::Text> player_info_;

    std::vector<std::shared_ptr<Button>> ability_buttons_;
public:
    UpgradePanel(std::shared_ptr<RenderService> render_service,
        std::shared_ptr<AudioService> audio_service, std::shared_ptr<Character> player);

    void update(float deltaTime) override;
    void render() override;
    void handleInput(const sf::Event& evt) override;
    void updatePlayerInfo();
};
