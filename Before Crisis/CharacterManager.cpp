#include "CharacterManager.h"


Character::Character() : level(1), exp(0), ability_points_(1), money_(50), rank_(3) {
    stats.hp = 100;
    stats.mp = 50;
    stats.strength = 10;

    AddItem({ 0, "Pistol", "weapon" , 5, "dmg"});
    AddItem({ 10, "Suit", "armor",-5,"dmg" });
    AddItem({ 20,"Medkit", "consumable",5,"hp" });
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
        if (item.instanceID == itemId) {
            if (item.type == "weapon") {
				if (equippedWeapon_ && item.instanceID == equippedWeapon_->instanceID)
					equippedWeapon_ = std::nullopt;
                else
					equippedWeapon_ = item;
            }
            else if (item.type == "armor") {
				if (equippedArmor_ && item.instanceID == equippedArmor_->instanceID)
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
        if (item.instanceID == itemId) {
            if (item.type == "weapon" && (!equippedWeapon_ || equippedWeapon_->instanceID == item.instanceID)) {
                return true;
            }
            else if (item.type == "armor" && (!equippedArmor_ || equippedArmor_->instanceID == item.instanceID)) {
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

bool Character::RemoveItem(int id) {
    auto it = std::find_if(inventory_.begin(), inventory_.end(),
        [id](const Item& item) { return item.id == id; });
    if (it != inventory_.end()) {
		if (equippedWeapon_ && equippedWeapon_->id == id) {
			equippedWeapon_.reset();
		}
        inventory_.erase(it);
        return true;
    }
    return false;
}