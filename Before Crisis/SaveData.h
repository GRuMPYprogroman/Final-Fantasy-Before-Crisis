#pragma once
#include <string>
#include <vector>
#include <optional>
#include "GameUtils.h"
#include "json.hpp"

struct SaveData {
    int level = 1;
    int exp = 0;

    int hp = 100;
    int mp = 50;
    int strength = 10;

    std::vector<Item> inventory;

    int equippedWeaponId;
    int equippedArmorId;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Item, id, name, type)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SaveData,
    level, exp,
    hp, mp, strength,
    inventory,
    equippedWeaponId, equippedArmorId)
