#pragma once
#include "Level.h"
#include "Monster.h"
#include <vector>

class Level;
class Monster;

namespace GameUtils
{
    static std::vector<MonsterType> GetMonstersForArea(AreaType area);
    static std::vector<Monster> SpawnEnemies(const Level& level, int numEnemies);
    //static std::vector<std::pair<int, int>> GetFloorTiles(const Level& level);
}