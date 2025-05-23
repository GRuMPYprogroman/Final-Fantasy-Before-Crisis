#pragma once
#include <string>

class Monster
{
private:
    int hp_;
    int dmg_;
    int exp_;
    std::string name_;
    int sex_;
public:
	Monster(int hp, int dmg, int exp, std::string& name_, int sex);
    int GetHp() const { return hp_; }
    int GetDmg() const { return dmg_; }
    int getExp() const { return exp_; }
    int getSex() const { return sex_; }
    std::string GetName() const { return name_; }
    void TakeDamage(float damage) { hp_ -= damage; }
};