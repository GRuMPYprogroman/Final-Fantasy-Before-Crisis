#pragma once
#include <string>

enum class MonsterType { Goblin, Dragon, Wolf, Bear, Bat, Spider, Troll };

class Monster
{
private:
    MonsterType type;
    int x;
    int y;
    int health;
    int attack;
    std::string name;
public:
	Monster(MonsterType type, int x, int y);
    std::string GetName() const { return name; }
    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetHealth() const { return health; }
    int GetAttack() const { return attack; }
    void TakeDamage(int damage) { health -= damage; }
};