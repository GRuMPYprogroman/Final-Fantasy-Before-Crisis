#pragma once
#include "IGameState.h"
#include "RenderService.h"
#include "AudioService.h"
#include "StateService.h"
#include "CharacterManager.h"
#include "CombatState.h"
#include "GameUtils.h"
#include "Location.h"
#include <deque>
#include <string>
#include "string_view"
#include <vector>
#include <map>
#include <random>

class ExplorationState : public IGameState {
private:
    std::shared_ptr<RenderService> render_;
    std::shared_ptr<AudioService> audio_;
    std::shared_ptr<StateService> states_;
    std::shared_ptr<Character> player_;
    std::function<void(bool)> onFinish_;

    sf::RectangleShape messageArea_;
    sf::RectangleShape inputArea_;
    std::unique_ptr<sf::Text> messageText_;
    std::unique_ptr<sf::Text> inputPrompt_;
    std::unique_ptr<sf::Font> font_;
    std::string inputBuffer_;
    std::deque<std::string> logLines_;
    unsigned int fontSize_ = 18;

    int locationId_;
    int total_enemies_;

    std::map<int, Location> locationsMap_;
    std::map<int, Monster> enemies_;
    Location current_loc;

    int current_hp_;

    std::mt19937 mt_randomizer_;

    void loadLocations(std::string_view path);
    void initUI();

    void addLog(std::string_view line);
    std::vector<std::string> wrapText(std::string_view text, float maxWidth);

    Monster pickRandomEnemy(std::vector<Monster>& possible_enemies);

    void processCommand(std::string cmd);
    void describeRoom();
    void finishMission(bool victory);
public:
    ExplorationState(std::shared_ptr<RenderService> renderService,
        std::shared_ptr<AudioService>  audioService,
        std::shared_ptr<StateService>  stateService,
        std::shared_ptr<Character> player,
        int locationId,
        std::function<void(bool)> onFinish);

    void update(float& deltaTime) override;
    void render() override;
    void handleInput(const sf::Event& evt) override;

    friend void loadMap(ExplorationState& obj,std::string_view pathToLocation);
    friend void loadEnemies(ExplorationState& obj,int min_enemies,int max_enemies, std::string_view pathToEnemies);
};
