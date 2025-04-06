#include "Monster.h"

Monster::Monster(MonsterType type, int x, int y, int health, int attack, std::string name) :
type(type),
x(x),
y(y),
health(health),
attack(attack),
name(name)
{}