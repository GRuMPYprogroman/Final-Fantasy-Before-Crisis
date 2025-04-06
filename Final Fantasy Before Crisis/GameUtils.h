#pragma once
#include "Level.h"
#include "Monster.h"
#include <vector>

namespace GameUtils {
    std::vector<MonsterType> GetMonstersForArea(AreaType area);
    std::vector<Monster> SpawnEnemies(const Level& level, int numEnemies);
}