#include "CharacterManager.h"

Character::Character() : level(1), exp(0)
{
	stats.hp = 100;
	stats.mp = 50;
	stats.strength = 10;
}

void Character::GainExp(int amount)
{
	exp += amount;
	if (exp >= level * 100)
	{
		level++;
		stats.hp += 10;
		stats.strength += 2;
		exp = 0;
	}
}

void Character::EquipItem(const std::string& item)
{
	//later
}
