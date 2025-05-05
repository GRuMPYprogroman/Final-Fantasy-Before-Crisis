#include "ContractsPanel.h"
#include "ExplorationState.h"
#include <fstream>
#include <sstream>
#include <iostream>

ContractsPanel::ContractsPanel(
    std::shared_ptr<RenderService> render_service,
    std::shared_ptr<AudioService> audio_service,
    std::shared_ptr<StateService> state_service,
    std::shared_ptr<Character> player,
    const std::string& csvPath)
    : render_service_(render_service),
      audio_service_(audio_service),
	  state_service_(state_service),
      player_(player)
{
    font_ = std::make_unique<sf::Font>(render_service_->getDefaultFont());
    title_ = std::make_unique<sf::Text>(*font_, "Contracts", 32);
    title_->setFillColor(sf::Color::White);
    title_->setPosition({ 20.f, 20.f });

    loadContracts(csvPath);
    buildButtons();
}

void ContractsPanel::loadContracts(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ContractsPanel: cannot open " << path << "\n";
        return;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        Contract c;
        if (!std::getline(ss, cell, ';')) continue;
        c.id = std::stoi(cell);
        if (!std::getline(ss, cell, ';')) continue;
        c.title = cell;
        if (!std::getline(ss, cell, ';')) continue;
        c.required_rank = std::stoi(cell);
        if (!std::getline(ss, cell, ';')) continue;
        c.reward_exp = std::stoi(cell);
        if (!std::getline(ss, cell, ';')) continue;
        c.reward_money = std::stoi(cell);
        if (!std::getline(ss, cell, ';')) continue;
        c.min_enemies = std::stoi(cell);
        if (!std::getline(ss, cell, ';')) continue;
        c.max_enemies = std::stoi(cell);

        all_contracts_.push_back(c);
    }
}

void ContractsPanel::buildButtons() {

    sf::Vector2f pos = sf::Vector2f{ render_service_->getWindowSize() };
	float centerX = pos.x / 2.f - 100.f;
	float centerY = pos.y * 3.f / 8.f - 100.f;

    float spacing = 100.f;
    int playerRank = player_->getRank();

    for (auto& c : all_contracts_) {
        if (c.required_rank < playerRank) continue;

        std::string label = c.title +
            "\nReq.rank: " + std::to_string(c.required_rank) +
            ",\nExp: " + std::to_string(c.reward_exp) + 
            ",\nMoney: " + std::to_string(c.reward_money);

        auto btn = std::make_shared<Button>(
            label,
            *font_,
            sf::Vector2f(centerX, centerY),
            [this, &id = c.id]() {
                audio_service_->playSound(SoundID::Click);
				state_service_->popState();
                state_service_->pushState(
                    std::make_unique<ExplorationState>()
                );
            },
            audio_service_
        );
        contract_buttons_.push_back(btn);
        centerY += spacing;
    }
}

void ContractsPanel::update(float dt) {
    for (auto& b : contract_buttons_) b->update();
}

void ContractsPanel::render() {
    render_service_->getRenderWindow().draw(*title_);
    for (auto& b : contract_buttons_) {
        render_service_->getRenderWindow().draw(b->getBackground());
        render_service_->getRenderWindow().draw(b->getLabel());
    }
}

void ContractsPanel::handleInput(const sf::Event& evt) {
    for (auto& b : contract_buttons_)
        b->handleInput(evt, render_service_->getRenderWindow());
}
