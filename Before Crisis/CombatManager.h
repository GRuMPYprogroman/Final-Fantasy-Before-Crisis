#pragma once
#include "CharacterManager.h"
#include "Monster.h"
#include "ICombatManager.h"

class Monster;
class Character;

class CombatManager : public ICombatManager
{
public:
	void StartCombat(Character& player, Monster& monster) override;
};

