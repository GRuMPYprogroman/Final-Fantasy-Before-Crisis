#include "CharacterManager.h"

Character::Character() : level(1), exp(0), ability_points_(0), money_(50) {
    stats.hp = 100;
    stats.mp = 50;
    stats.strength = 10;

    inventory_ = {
        { 0, "Pistol", "weapon"},
        { 10, "Suit", "armor"},
        { 20,"Medkit", "consumable"}
    };
}

void Character::GainExp(int amount) {
    exp += amount;
    if (exp >= level * 100) {
        level++;
        stats.hp += 10;
        stats.strength += 2;
        exp = 0;
    }
}

void Character::EquipItem(const int itemId) {
    for (const auto& item : inventory_) {
        if (item.id == itemId) {
            if (item.type == "weapon") {
				if (equippedWeapon_ && item.id == equippedWeapon_->id)
					equippedWeapon_ = std::nullopt;
                else
					equippedWeapon_ = item;
            }
            else if (item.type == "armor") {
				if (equippedArmor_ && item.id == equippedArmor_->id)
					equippedArmor_ = std::nullopt;
				else
					equippedArmor_ = item;
            }
            return;
        }
    }
}

bool Character::canEquip(const int itemId)
{
    for (const auto& item : inventory_) {
        if (item.id == itemId) {
            if (item.type == "weapon" && (!equippedWeapon_ || equippedWeapon_->id == item.id)) {
                return true;
            }
            else if (item.type == "armor" && (!equippedArmor_ || equippedArmor_->id == item.id)) {
                return true;
            }
            else
                return false;
        }
    }
}


SaveData Character::ToSaveData() const {
    SaveData data;
    data.level = level;
    data.exp = exp;
    data.hp = stats.hp;
    data.mp = stats.mp;
    data.strength = stats.strength;

    data.inventory = inventory_;

    if (equippedWeapon_) data.equippedWeaponId = equippedWeapon_->id;
    if (equippedArmor_) data.equippedArmorId = equippedArmor_->id;

    return data;
}

void Character::FromSaveData(const SaveData& data) {
    level = data.level;
    exp = data.exp;
    stats.hp = data.hp;
    stats.mp = data.mp;
    stats.strength = data.strength;

    inventory_ = data.inventory;

    equippedWeapon_.reset();
    equippedArmor_.reset();

    for (const auto& item : inventory_) {
        if (data.equippedWeaponId && item.id == data.equippedWeaponId) {
            equippedWeapon_ = item;
        }
        if (data.equippedArmorId && item.id == data.equippedArmorId) {
            equippedArmor_ = item;
        }
    }
}
