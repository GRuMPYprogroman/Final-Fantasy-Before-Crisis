#pragma once
#include "Level.h"
#include "Monster.h"
#include <vector>

struct Item {
    int id;
    std::string name;
    std::string type;
    int effect;
    std::string effect_type;
    int instanceID;
};

class Level;
class Monster;

static std::vector<MonsterType> GetMonstersForArea(AreaType area);
static std::vector<Monster> SpawnEnemies(const Level& level, int numEnemies);
//static std::vector<std::pair<int, int>> GetFloorTiles(const Level& level);