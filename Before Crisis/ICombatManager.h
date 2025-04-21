#pragma once
#include "Monster.h"

class Monster;

class ICombatManager
{
public:
	virtual ~ICombatManager() = default;
	virtual void StartCombat(Character& player, Monster& monster) = 0;
};

