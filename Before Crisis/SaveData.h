#pragma once
#include "json.hpp"

struct SaveData {
    int playerX = 0;
    int playerY = 0;
    int playerHP = 100;
    //(mission progress, inventory)
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SaveData, playerX, playerY, playerHP);