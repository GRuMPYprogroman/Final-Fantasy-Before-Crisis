#include "InventoryPanel.h"

InventoryPanel::InventoryPanel(
    std::shared_ptr<RenderService> renderService,
    std::shared_ptr<AudioService> audio_service,
    std::shared_ptr<Character> player)
    : render_service_(renderService)
    , audio_service_(audio_service)
    , player_(player)
{
    font_ = std::make_unique<sf::Font>(render_service_->getDefaultFont());
    title_ = std::make_unique<sf::Text>(*font_);
    title_->setString("Inventory");
    title_->setCharacterSize(28);
    title_->setFillColor(sf::Color::White);
    title_->setPosition({ 20.f, 20.f });

    std::vector<Item> weapons;
    std::vector<Item> armors;
    std::map<int, std::pair<Item, int>> consumables;

    for (const auto& item : player_->GetInventory()) {
        if (item.type == "weapon") {
            weapons.push_back(item);
        }
        else if (item.type == "armor") {
            armors.push_back(item);
        }
        else if (item.type == "consumable") {
            auto& entry = consumables[item.id];
            entry.first = item;
            entry.second += 1;
        }
    }

    float winW = render_service_->getRenderWindow().getSize().x;
    float winH = render_service_->getRenderWindow().getSize().y;
    float colW = winW / 3.f;
    const float topY = winH * 3.f / 4.f / 8.f;
    const float spacingY = 60.f;

    struct Col { std::string name; float x; };
    std::vector<Col> cols = {
        {"Weapons",     colW * 0 + 20.f},
        {"Armors",      colW * 1 + 20.f},
        {"Consumables", colW * 2 + 20.f}
    };

    for (auto& c : cols) {
        auto lbl = std::make_unique<sf::Text>(*font_, c.name, 24);
        lbl->setPosition({ c.x, topY });
        lbl->setFillColor(sf::Color(180, 180, 50));
        column_labels_.push_back(std::move(lbl));
    }

    float y = topY + 40.f;
    for (auto& itm : weapons) {
        auto btn = std::make_shared<Button>(
            itm.name, *font_, sf::Vector2f{ cols[0].x, y },
            [](){},
            audio_service_);

        btn->setCallback([itm, this, btn]() {
            if (player_->canEquip(itm.instanceID)) {
                audio_service_->playSound(SoundID::Inventory);
                player_->EquipItem(itm.instanceID);
                btn->setChosen();
            }
            });

        if (player_->GetEquippedWeapon() &&
            player_->GetEquippedWeapon()->instanceID == itm.instanceID)
        {
            btn->setChosen();
        }
        item_buttons_.push_back(btn);
        y += spacingY;
    }

    y = topY + 40.f;
    for (auto& itm : armors) {
        auto btn = std::make_shared<Button>(
            itm.name, *font_, sf::Vector2f{ cols[1].x, y },
            [](){},
            audio_service_);

        btn->setCallback([itm, this, btn]() {
            if (player_->canEquip(itm.instanceID)) {
                audio_service_->playSound(SoundID::Inventory);
                player_->EquipItem(itm.instanceID);
                btn->setChosen();
            }
            });

        if (player_->GetEquippedArmor() &&
            player_->GetEquippedArmor()->instanceID == itm.instanceID)
        {
            btn->setChosen();
        }
        item_buttons_.push_back(btn);
        y += spacingY;
    }

    y = topY + 40.f;
    for (auto& [id, pair] : consumables) {
        const Item& itm = pair.first;
        int count = pair.second;
        std::string label = itm.name + " x" + std::to_string(count);

        auto btn = std::make_shared<Button>(
            label, *font_, sf::Vector2f{ cols[2].x, y },
            []() {},
            audio_service_);
        item_buttons_.push_back(btn);
        y += spacingY;
    }

    for (auto& btn : item_buttons_)
        btn->setBackgroundSize(btn->getBackgroundSize() + sf::Vector2f(15.f, 0.f));
}

void InventoryPanel::update(float deltaTime) {
    for (auto& btn : item_buttons_)
        btn->update();
}

void InventoryPanel::render() {
    auto& w = render_service_->getRenderWindow();
    w.draw(*title_);
    for (auto& lbl : column_labels_)
        w.draw(*lbl);
    for (auto& btn : item_buttons_) {
        w.draw(btn->getBackground());
        w.draw(btn->getLabel());
    }
}

void InventoryPanel::handleInput(const sf::Event& event) {
    for (auto& btn : item_buttons_)
        btn->handleInput(event, render_service_->getRenderWindow());
}
