#pragma once
#include "IGameplayPanel.h"
#include "RenderService.h"
#include "AudioService.h"
#include "CharacterManager.h"
#include "Button.h"
#include "GameUtils.h"
#include <vector>

class ShopPanel : public IGameplayPanel {
private:
    struct ShopItem { Item data; int price; int required_level; };

    void loadItemsFromCSV(const std::string& path);
    void buildButtons();

    std::shared_ptr<RenderService> render_service_;
    std::shared_ptr<AudioService>  audio_service_;
    std::shared_ptr<Character>     player_;

    std::shared_ptr<sf::Font>     font_;
    std::unique_ptr<sf::Text>     title_;
    std::unique_ptr<sf::Text> player_info_;

    std::vector<ShopItem>         shopItems_;
    std::map<std::string, std::vector<ShopItem>> groupedItems_;
    std::vector<std::shared_ptr<Button>> itemButtons_;
    std::vector<std::unique_ptr<sf::Text>> columnHeaders_;


    std::shared_ptr<Button> buyButton_;
    std::shared_ptr<Button> sellButton_;
    std::shared_ptr<Button> closeButton_;
    std::unique_ptr<sf::RectangleShape> popupBackground_;

    std::optional<ShopItem> selectedItem_;
    bool selection_locked_ = false;
    sf::Vector2f button_group_pos_;
public:
    ShopPanel(std::shared_ptr<RenderService> renderService,
        std::shared_ptr<AudioService> audioService,
        std::shared_ptr<Character> player,
        const std::string& csvPath);

    void update(float deltaTime) override;
    void render() override;
    void handleInput(const sf::Event& evt) override;
    void createActionButtons();
    void updatePlayerInfo();
};
