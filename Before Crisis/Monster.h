#pragma once
#include <string>

class Monster
{
private:
    int hp_;
    int dmg_;
    int exp_;
    std::string name_;
public:
	Monster(int hp, int dmg, int exp, std::string& name_);
    int GetHp() const { return hp_; }
    int GetDmg() const { return dmg_; }
    int getExp() const { return exp_; }
    std::string GetName() const { return name_; }
    void TakeDamage(int damage) { hp_ -= damage; }
};