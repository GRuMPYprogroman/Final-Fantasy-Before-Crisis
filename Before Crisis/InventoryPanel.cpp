#include "InventoryPanel.h"

InventoryPanel::InventoryPanel(std::shared_ptr<RenderService> renderService,std::shared_ptr<AudioService> audio_service, std::shared_ptr<Character> player)
    : render_service_(renderService),audio_service_(audio_service) ,player_(player) {

    font_ = std::make_unique<sf::Font>(render_service_->getDefaultFont());

	title_ = std::make_unique<sf::Text>(*font_);
    title_->setFont(*font_);
    title_->setString("Inventory");
    title_->setCharacterSize(28);
    title_->setFillColor(sf::Color::White);
    title_->setPosition(sf::Vector2f{ 20.f, 20.f });

    std::map<std::string, std::vector<Item>> grouped;
    for (const auto& item : player_->GetInventory()) {
        grouped[item.type].push_back(item);
    }

    const float colWidth = 250.f;
    const float spacingY = 100.f;

    float winX = render_service_->getRenderWindow().getSize().x - colWidth;
    float winY = render_service_->getRenderWindow().getSize().y;
    winX /= 3.f;

    const float topY = winY * 3.f / 4.f / 8.f;

    std::vector<std::pair<std::string, float>> columns = {
        {"weapon",      winX - 150.f},
        {"armor",      2 * winX - 150.f},
        {"consumable",  3 * winX - 150.f}
    };

    for (const auto& [type, xPos] : columns) {
        auto label = std::make_unique<sf::Text>(*font_,type + "s",24);
        label->setPosition({ xPos, topY });
        label->setFillColor(sf::Color(180, 180, 50));
        column_labels_.push_back(std::move(label));

        float y = topY + 40.f;
        for (const auto& item : grouped[type]) {
            std::string text = item.name;
            auto button = std::make_shared<Button>(
                text, *font_, sf::Vector2f(xPos, y),
                []() {}, audio_service_
            );

            button->setCallback([item, this, button]() {
                if (player_->canEquip(item.instanceID)) {
                    audio_service_->playSound(SoundID::Inventory);
                    player_->EquipItem(item.instanceID);
                    button->setChosen();
                }
                });

            if (player_->GetEquippedArmor())
                if (player_->GetEquippedArmor()->instanceID == item.instanceID)
                    button->setChosen();

            if (player_->GetEquippedWeapon())
                if (player_->GetEquippedWeapon()->instanceID == item.instanceID)
                    button->setChosen();

            item_buttons_.push_back(button);
            y += spacingY;
        }
    }
}

void InventoryPanel::update(float deltaTime) {
    for (auto& btn : item_buttons_)
        btn->update();
}

void InventoryPanel::render() {
    render_service_->getRenderWindow().draw(*title_);
    for (auto& label : column_labels_)
        render_service_->getRenderWindow().draw(*label);
    for (auto& btn : item_buttons_)
    {
        render_service_->getRenderWindow().draw(btn->getBackground());
        render_service_->getRenderWindow().draw(btn->getLabel());
    }
}

void InventoryPanel::handleInput(const sf::Event& event) {
    for (auto& btn : item_buttons_)
        btn->handleInput(event, render_service_->getRenderWindow());
}