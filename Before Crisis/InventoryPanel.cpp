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

    const float startY = 80.f;
    float offsetY = 0.f;

    for (const auto& item : player_->GetInventory()) {
        std::shared_ptr<Button> button = std::make_shared<Button>(
            item.name, *font_, sf::Vector2f(200, startY + offsetY),
            []() {}, audio_service_
        );

        button->setCallback([item, this, button]() {
            if (player_->canEquip(item.id)){
                audio_service_->playSound(SoundID::Inventory);
                player_->EquipItem(item.id);
                button->setChosen();
            }});

        item_buttons_.push_back(button);
        offsetY += 50.f;
    }
}

void InventoryPanel::update(float deltaTime) {
    for (auto& btn : item_buttons_)
        btn->update();
}

void InventoryPanel::render() {
    render_service_->getRenderWindow().draw(*title_);
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