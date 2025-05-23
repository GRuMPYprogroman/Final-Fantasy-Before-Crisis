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
    int successful_contracts_;

    std::vector<Item> inventory_;
    Item* equippedWeapon_;
    Item* equippedArmor_;

    int next_item_id_ = 0;
public:
    Character();

    void EquipItem(const int itemId);
    bool canEquip(const int itemId);

    int GetLevel() const { return level; }
    void GainExp(int amount);
    int getExp() const { return exp; }
    int getMoney() const { return money_; }
	int getAbilityPoints() const { return ability_points_; }
    int getRank() const { return rank_; }
    int getCompletedMissions() const { return successful_contracts_; }
    Stats GetStats() const { return stats; }
    const std::vector<Item>& GetInventory() const { return inventory_; }
    Item* GetEquippedWeapon() const { return equippedWeapon_; }
    Item* GetEquippedArmor() const { return equippedArmor_; }
    void setMoney(int toAdd) { money_ += toAdd; }
	void setAbilityPoints(int points) { ability_points_ += points; }
	void setHp(int hp) { stats.hp += hp; }
	void setMp(int mp) { stats.mp += mp; }
	void setStrength(int strength) { stats.strength += strength; }
    void setRank(int rank) { (rank_ == 1) ? rank_ = 1 : rank_ -= rank; }
    void setContracts(int amount) { successful_contracts_ += amount; }

    bool RemoveItem(int id);
    void AddItem(Item item) { item.instanceID = next_item_id_; inventory_.push_back(item); ++next_item_id_; }

    SaveData ToSaveData() const;
    void FromSaveData(const SaveData& data);
};