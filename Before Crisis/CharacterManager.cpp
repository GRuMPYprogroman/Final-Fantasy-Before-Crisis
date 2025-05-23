#include "CharacterManager.h"
#include "iostream"
#define startRank = 4
 

Character::Character() : level(10), exp(0), ability_points_(1), money_(5000), rank_(4), successful_contracts_(0){
    stats.hp = 100000;
    stats.mp = 50;
    stats.strength = 10;

    AddItem({ 0, "Pistol", "weapon" , 5, "dmg",1,6});
    AddItem({ 10, "Suit", "armor",-5,"dmg" ,2,-1 });
    AddItem({ 20,"Medkit", "consumable",5,"hp",3,-1 });
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
					equippedWeapon_ = nullptr;
                else
					equippedWeapon_ = new Item(item);
            }
            else if (item.type == "armor") {
				if (equippedArmor_ && item.instanceID == equippedArmor_->instanceID)
					equippedArmor_ = nullptr;
				else
					equippedArmor_ = new Item(item);
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

    try {
        std::ifstream file("../saveData/PlayerSaveData.json");
        if (file.is_open()) {
            nlohmann::json j;
            file >> j;
            file.close();

            SaveData existing = j.get<SaveData>();
            data.story_flags = existing.story_flags;
        }
    }
    catch (...) {
        std::cout << "Warning: couldn't load existing story flags, using defaults.\n";
    }

    data.level = level;
    data.exp = exp;
    data.rank = rank_;
    data.ability_points = ability_points_;
    data.money = money_;
    data.successful_contracts = successful_contracts_;
    data.hp = stats.hp;
    data.mp = stats.mp;
    data.strength = stats.strength;

    data.inventory = inventory_;

    if (equippedWeapon_) data.equippedWeaponId = equippedWeapon_->id;
    else data.equippedWeaponId = -1;

    if (equippedArmor_) data.equippedArmorId = equippedArmor_->id;
    else data.equippedArmorId = -1;

    return data;
}

void Character::FromSaveData(const SaveData& data) {
    level = data.level;
    exp = data.exp;
    stats.hp = data.hp;
    stats.mp = data.mp;
    stats.strength = data.strength;
    rank_ = data.rank;
    ability_points_ = data.ability_points ;
    money_ = data.money;
    successful_contracts_ = data.successful_contracts;

    inventory_ = data.inventory;

    equippedWeapon_ = nullptr;
    equippedArmor_ = nullptr;

    for (const auto& item : inventory_) {
        if (data.equippedWeaponId && item.id == data.equippedWeaponId) {
            equippedWeapon_ = new Item(item);
        }
        if (data.equippedArmorId && item.id == data.equippedArmorId) {
            equippedArmor_ = new Item(item);
        }
    }
}

bool Character::RemoveItem(int id) {
    auto it = std::find_if(inventory_.begin(), inventory_.end(),
        [id](const Item& item) { return item.id == id; });
    if (it != inventory_.end()) {
		if (equippedWeapon_ && equippedWeapon_->id == id) {
			equippedWeapon_ = nullptr;
		}
        if (equippedArmor_ && equippedArmor_->id == id) {
            equippedArmor_ = nullptr;
        }
        inventory_.erase(it);
        return true;
    }
    return false;
}