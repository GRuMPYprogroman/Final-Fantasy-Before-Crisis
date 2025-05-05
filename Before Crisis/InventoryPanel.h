#pragma once
#include <vector>
#include <memory>
#include "IGameplayPanel.h"
#include "RenderService.h"
#include "Button.h"
#include "CharacterManager.h"

class InventoryPanel : public IGameplayPanel {
private:
    std::shared_ptr<RenderService> render_service_;
	std::shared_ptr<AudioService> audio_service_;
    std::shared_ptr<Character> player_;
    std::vector<std::shared_ptr<Button>> item_buttons_;
    std::unique_ptr<sf::Text> title_;
    std::unique_ptr<sf::Font> font_;
    std::vector<std::unique_ptr<sf::Text>> column_labels_;
public:
    InventoryPanel(std::shared_ptr<RenderService> renderService, std::shared_ptr<AudioService> audio_service,std::shared_ptr<Character> player);

    void update(float deltaTime) override;
    void render() override;
    void handleInput(const sf::Event& event) override;
};


