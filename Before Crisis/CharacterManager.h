#pragma once
#include <string>
#include <vector>
#include <optional>
#include "SaveData.h"

struct Stats {
    int hp;
    int mp;
    int strength;
};


class Character {
private:
    Stats stats;
    int level;
    int exp;
    int ability_points_;
    int money_;

    std::vector<Item> inventory_;
    std::optional<Item> equippedWeapon_;
    std::optional<Item> equippedArmor_;

public:
    Character();

    void EquipItem(const int itemId);
    bool canEquip(const int itemId);
    void GainExp(int amount);
    void TakeDamage(int damage) { stats.hp -= damage; }
    int getMoney() const { return money_; }
    void setMoney(int toAdd) { money_ += toAdd; }

    Stats GetStats() const { return stats; }
    int GetLevel() const { return level; }

    const std::vector<Item>& GetInventory() const { return inventory_; }

    std::optional<Item> GetEquippedWeapon() const { return equippedWeapon_; }
    std::optional<Item> GetEquippedArmor() const { return equippedArmor_; }

    void AddItem(const Item& item) { inventory_.push_back(item); }

    SaveData ToSaveData() const;
    void FromSaveData(const SaveData& data);
};