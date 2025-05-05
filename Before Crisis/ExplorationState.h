#pragma once
#include "IGameState.h"
#include "RenderService.h"
#include "AudioService.h"
#include "StateService.h"
#include "CharacterManager.h"
#include "CombatState.h"
#include "MonsterType.h"
#include "GameUtils.h"
#include <deque>
#include <string>
#include "string_view"
#include <vector>
#include <map>
#include <random>

struct LocationData {
    std::string name;
    std::string description;
    std::array<int, 4> neighbors;
    std::string lootTable;
    std::string enemyTable;
    std::string backgroundImage;
    std::string musicTrack;
};

class ExplorationState : public IGameState {
private:
    sf::RectangleShape messageArea_;
    sf::RectangleShape inputArea_;
    std::unique_ptr<sf::Text> messageText_;
    std::unique_ptr<sf::Text> inputPrompt_;
    std::unique_ptr<sf::Font> font_;
    std::string inputBuffer_;
    std::deque<std::string> logLines_;
    unsigned int fontSize_ = 18;

    int locationId_;
    LocationData locData_;
    std::vector<std::pair<MonsterType, int>> enemyWeights_;
    std::vector<std::pair<Item, int>> lootWeights_;

    std::shared_ptr<RenderService> render_;
    std::shared_ptr<AudioService> audio_;
    std::shared_ptr<StateService> states_;
    std::shared_ptr<Character> player_;

    std::map<int, LocationData> locationsMap_;
    std::map<std::string, std::vector<std::pair<MonsterType, int>>> enemiesMap_;
    std::map<std::string, std::vector<std::pair<Item, int>>> lootMap_;

    std::mt19937 rng_;

    void loadLevel(int contractId);
    void loadLocations(std::string_view path);
    void loadEnemies(std::string_view path);
    void loadLoot(std::string_view path);
    void initUI();

    void addLog(std::string_view line);
    std::vector<std::string> wrapText(std::string_view text, float maxWidth);
    MonsterType pickRandomEnemy();
    Item pickRandomLoot();

    void processCommand(std::string cmd);
    void describeRoom();
    void tryRandomEvent();
public:
    ExplorationState(std::shared_ptr<RenderService> renderService,
        std::shared_ptr<AudioService>  audioService,
        std::shared_ptr<StateService>  stateService,
        std::shared_ptr<Character> player,
        int locationId,
        std::string_view locationsCsv,
        std::string_view enemiesCsv,
        std::string_view lootCsv);

    void update(float& deltaTime) override = 0;
    void render() override;
    void handleInput(const sf::Event& evt) override;
};
