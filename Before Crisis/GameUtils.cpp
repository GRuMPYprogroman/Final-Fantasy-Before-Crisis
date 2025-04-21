#include "GameUtils.h"
#include <algorithm>
#include <random>

/**
namespace GameUtils {
    std::vector<MonsterType> GetMonstersForArea(AreaType area) {
        switch (area) {
        case AreaType::Forest:
            return { MonsterType::Wolf, MonsterType::Bear, MonsterType::Goblin };
        case AreaType::Cave:
            return { MonsterType::Bat, MonsterType::Spider, MonsterType::Troll };
        case AreaType::Dungeon:
            return { MonsterType::Goblin, MonsterType::Dragon };
        default:
            return {};
        }
    }
std::vector<Monster> SpawnEnemies(const Level& level, int numEnemies) {
        std::vector<Monster> enemies;
        auto possibleMonsters = GetMonstersForArea(level.GetAreaType());
        auto floorTiles = GetFloorTiles(level);

        std::random_device rd;
        std::mt19937 gen(rd());

        std::shuffle(floorTiles.begin(), floorTiles.end(),gen);
        int spawnCount = std::min(numEnemies, static_cast<int>(floorTiles.size()));

        for (int i = 0; i < spawnCount; ++i) {
            MonsterType type = possibleMonsters[rand() % possibleMonsters.size()];
            enemies.emplace_back(type, floorTiles[i].first, floorTiles[i].second);
        }
        return enemies;
    }
}**/