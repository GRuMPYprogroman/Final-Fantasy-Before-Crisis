#pragma once
#include <string>
#include <vector>
#include "StoryPoints.h"
#include "GameUtils.h"
#include "json.hpp"
using nlohmann::json;

constexpr int flags_amount = static_cast<int>(story_flags_::number_of_flags);

struct SaveData {
   int level = 1;
   int exp = 0;
   int rank = 3;
   int ability_points = 0;
   int money = 50;
   int successful_contracts = 0.3;

   int hp = 100;
   int mp = 50;
   int strength = 10;

   std::vector<Item> inventory = {
	   { 0, "Pistol", "weapon" , 5, "dmg" },
	   { 10, "Suit", "armor",-5,"dmg" },
	   { 20,"Medkit", "consumable",5,"hp" } };

   int equippedWeaponId = -1;
   int equippedArmorId = -1;

   std::vector<bool> story_flags = std::vector<bool>(static_cast<size_t>(story_flags_::number_of_flags), false);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Item, id, name, type)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SaveData,
   level, exp, rank,
   hp, mp, strength,
   ability_points, money, successful_contracts,
   inventory,
   equippedWeaponId, equippedArmorId,
   story_flags)
