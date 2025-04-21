#pragma once
#include "string"

struct Stats
{
	int hp;
	int mp;
	int strength;
};

class Character {
private:
	Stats stats;
	int level;
	int exp;
public:
	Character();
	void EquipItem(const std::string& item);
	void GainExp(int amount);
	void TakeDamage(int damage) { stats.hp -= damage; }
	Stats GetStats() const { return stats; }
	int GetLevel() const { return level; }
};

