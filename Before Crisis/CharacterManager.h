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
    int rank_;

    std::vector<Item> inventory_;
    std::optional<Item> equippedWeapon_;
    std::optional<Item> equippedArmor_;

    int next_item_id_ = 0;
public:
    Character();

    void EquipItem(const int itemId);
    bool canEquip(const int itemId);
    void GainExp(int amount);
    void TakeDamage(int damage) { stats.hp -= damage; }
    int getMoney() const { return money_; }
	int getAbilityPoints() const { return ability_points_; }
    int getRank() const { return rank_; }
    Stats GetStats() const { return stats; }
    const std::vector<Item>& GetInventory() const { return inventory_; }
    std::optional<Item> GetEquippedWeapon() const { return equippedWeapon_; }
    std::optional<Item> GetEquippedArmor() const { return equippedArmor_; }
    int GetLevel() const { return level; }
    void setMoney(int toAdd) { money_ += toAdd; }
	void setAbilityPoints(int points) { ability_points_ += points; }
	void setHp(int hp) { stats.hp += hp; }
	void setMp(int mp) { stats.mp += mp; }
	void setStrength(int strength) { stats.strength += strength; }
	void setRank(int rank) { rank_ = rank; }

    bool RemoveItem(int id);
    void AddItem(Item item) { item.instanceID = next_item_id_; inventory_.push_back(item); ++next_item_id_; }

    SaveData ToSaveData() const;
    void FromSaveData(const SaveData& data);
};