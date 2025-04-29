#include "ShopPanel.h"
#include <iostream>
#include <fstream>
#include <sstream>

ShopPanel::ShopPanel(std::shared_ptr<RenderService> renderService,
    std::shared_ptr<AudioService> audioService,
    std::shared_ptr<Character> player,
    const std::string& csvPath)
    : render_service_(renderService),
      audio_service_(audioService),
	  player_(player)
	{
		font_ = std::make_unique<sf::Font>(render_service_->getDefaultFont());

		title_ = std::make_unique<sf::Text>(*font_,"Shop",36);
		title_->setPosition({ render_service_->getWindowSize().x/2.f, 20.f });
		title_->setFillColor(sf::Color::White);

        player_info_ = std::make_unique<sf::Text>(
            *font_,
            "Money: " + std::to_string(player_->getMoney()) + "\n" + "Level: " + std::to_string(player_->GetLevel()),
            36
        );


        loadItemsFromCSV(csvPath);
        for (auto& si : shopItems_) {
            groupedItems_[si.data.type].push_back(si);
        }

		buildButtons();
	}

void ShopPanel::loadItemsFromCSV(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open CSV: " << path << "\n";
        return;
    }

    std::string line;
    if (!std::getline(file, line)) return;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        ShopItem si;
        // id
        if (!std::getline(ss, cell, ';')) continue;
        si.data.id = std::stoi(cell);

        // name
        if (!std::getline(ss, cell, ';')) continue;
        si.data.name = cell;

        // type
        if (!std::getline(ss, cell, ';')) continue;
        si.data.type = cell;

        // effect
        if (!std::getline(ss, cell, ';')) continue;
        si.data.effect = std::stoi(cell);

        //effect_type
        if (!std::getline(ss, cell, ';')) continue;
        si.data.effect_type = cell;

        // price
        if (!std::getline(ss, cell, ';')) continue;
        si.price = std::stoi(cell);

        if (!std::getline(ss, cell, ';')) continue;
        si.required_level = std::stoi(cell);

        shopItems_.push_back(si);
    }
    file.close();
}

void ShopPanel::buildButtons() {
    const float colWidth = 300.f;
    const float spacingY = 100.f;

    float winX = render_service_->getRenderWindow().getSize().x - colWidth;
    float winY = render_service_->getRenderWindow().getSize().y;
    winX /= 3.f;

    const float topY = winY * 3.f / 4.f / 8.f;

    std::vector<std::pair<std::string, float>> columns = {
        { "weapon",     winX  },
        { "armor",       2 * winX },
        { "consumable",  3 * winX }
    };

    for (auto& [type, xPos] : columns) {
        auto hdr = std::make_unique<sf::Text>(
            *font_,std::string(type).append("s"),28
        );
        hdr->setPosition({ xPos, topY });
        hdr->setFillColor(sf::Color(200, 200, 50));
        columnHeaders_.push_back(std::move(hdr));

        float y = topY + 40.f;
        for (auto& item : groupedItems_[type]) {
            std::string label = item.data.name + " - " + std::to_string(item.price) + "$\n" + "required level: " + std::to_string(item.required_level)
                + "\n" + "(" + std::to_string(item.data.effect) + "% " + item.data.effect_type + ")";
            auto btn = std::make_shared<Button>(
                label,
                *font_,
                sf::Vector2f(xPos, y),
                [this, item]() {
                    if (player_->getMoney() >= item.price && player_->GetLevel() >= item.required_level)
                    {
                        player_->AddItem(item.data);
                        player_->setMoney(-item.price);
                        player_info_->setString("Money: " + std::to_string(player_->getMoney()) + "\n" + "Level: " + std::to_string(player_->GetLevel()));
                        audio_service_->playSound(SoundID::Buy);
                    }
                },
                audio_service_
            );
            btn->setBackgroundSize(btn->getLabel().getGlobalBounds().size + sf::Vector2f{20.f,20.f});
            itemButtons_.push_back(btn);
            y += spacingY;
        }
    }
}

void ShopPanel::update(float deltaTime) {
    for (auto& btn : itemButtons_) btn->update();
}

void ShopPanel::render() {
    auto& win = render_service_->getRenderWindow();
    win.draw(*title_);
    win.draw(*player_info_);
    for (auto& hdr : columnHeaders_) win.draw(*hdr);
    for (auto& btn : itemButtons_) {
        win.draw(btn->getBackground());
        win.draw(btn->getLabel());
    }
}

void ShopPanel::handleInput(const sf::Event& evt) {
    for (auto& btn : itemButtons_)
        btn->handleInput(evt, render_service_->getRenderWindow());
}
