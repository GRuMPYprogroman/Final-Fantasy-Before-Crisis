#pragma once
#include "CharacterManager.h"
#include "Monster.h"

class CombatManager
{
public:
	void StartCombat(Character& player, Monster& monster);
};

