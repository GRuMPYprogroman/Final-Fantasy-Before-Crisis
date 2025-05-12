#include "ContractsPanel.h"
#include "ExplorationState.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define CONTRACT_TO_RANK_UP 5

ContractsPanel::ContractsPanel(
    std::shared_ptr<RenderService> render_service,
    std::shared_ptr<AudioService> audio_service,
    std::shared_ptr<StateService> state_service,
    std::shared_ptr<Character> player,
    const std::string& csvPath)
    : render_service_(render_service),
    audio_service_(audio_service),
    state_service_(state_service),
    player_(player),
    required_contract_id_(1)
{
    font_ = std::make_unique<sf::Font>(render_service_->getDefaultFont());
    title_ = std::make_unique<sf::Text>(*font_, "Contracts", 36);
    title_->setFillColor(sf::Color::White);
    title_->setPosition({ (render_service_->getWindowSize().x - title_->getGlobalBounds().size.x) / 2.f, 30.f});

    player_info_ = std::make_unique<sf::Text>(*font_, "", 28);
    player_info_->setPosition({ 30.f,title_->getGlobalBounds().size.y + 50.f});
    updatePlayerInfo();

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

        int contractId = c.id;
        int rewardExp = c.reward_exp;
        int rewardMoney = c.reward_money;

        std::string label = c.title +
            "\nReq.rank: " + std::to_string(c.required_rank) +
            ",\nExp: " + std::to_string(c.reward_exp) + 
            ",\nMoney: " + std::to_string(c.reward_money);

        auto btn = std::make_shared<Button>(
            label,
            *font_,
            sf::Vector2f(centerX, centerY),
            [this, contractId, rewardExp, rewardMoney]()
            {
                audio_service_->playSound(SoundID::Click);
				//state_service_->popState();
                state_service_->pushState(
                    std::make_unique<ExplorationState>(
                        render_service_,
                        audio_service_,
                        state_service_,
                        player_,
                        contractId,
                [this, rewardExp, rewardMoney, contractId](bool victory)
                        {
                            if (victory && contractId == required_contract_id_)
                            {
                                player_->setMoney(rewardMoney);
                                player_->GainExp(rewardExp);
                                player_->setContracts(1);
                                if (player_->getCompletedMissions() >= CONTRACT_TO_RANK_UP) 
                                {
                                    player_->setContracts(-CONTRACT_TO_RANK_UP);
                                    player_->setRank(1);
                                    ++required_contract_id_;
                                }
                                updatePlayerInfo();
                            }
                            else
                            {
                                player_->setContracts(-1);
                            }
                        }
                ));
            },
            audio_service_
        );
        btn->setBackgroundSize({ 120,120 });
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
    render_service_->getRenderWindow().draw(*player_info_);
}

void ContractsPanel::handleInput(const sf::Event& evt) {
    for (auto& b : contract_buttons_)
        b->handleInput(evt, render_service_->getRenderWindow());
}

void ContractsPanel::updatePlayerInfo()
{
    player_info_->setString("Successful contracts: " + std::to_string(player_->getCompletedMissions()) 
                           +"\nNeeded contracts of type "+ std::to_string(required_contract_id_) + " to rank up : " + std::to_string(CONTRACT_TO_RANK_UP));
}
