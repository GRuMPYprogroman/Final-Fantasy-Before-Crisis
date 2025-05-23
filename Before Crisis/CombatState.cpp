#include "CombatState.h"
#include <sstream>
#include <iostream>

CombatState::CombatState(
    std::shared_ptr<RenderService> renderService,
    std::shared_ptr<AudioService>  audioService,
    std::shared_ptr<StateService>  stateService,
    std::shared_ptr<Character>     player,
    Monster enemy,
    std::function<void(bool)> onFinish,
    int& currentHp)
    : render_service_(renderService),
      audio_service_(audioService),
      states_(stateService),
      player_(player),
	  enemy_(enemy),
      onFinish_(std::move(onFinish)),
      current_hp(currentHp)

{
    audio_service_->stopMusic();
    audio_service_->playMusic(MusicID::BattleTheme, 0);

    font_ = std::make_unique<sf::Font>(render_service_->getDefaultFont());
	txtPlayerHP_ = std::make_unique<sf::Text>(*font_);
	txtEnemyHP_ = std::make_unique<sf::Text>(*font_);

    txtPlayerHP_->setCharacterSize(20);
    txtPlayerHP_->setPosition({ 20, 20 });
    txtEnemyHP_->setCharacterSize(20);
    txtEnemyHP_->setPosition({ 20, 60 });

    float width = render_service_->getRenderWindow().getSize().x;
    float height = render_service_->getRenderWindow().getSize().y;

    float btnW = width / 2.f;
    float btnH = height * 3 / 4 + 30.f;
    float offset = 50.f;

    btnAttack_ = std::make_shared<Button>(
        "Attack", *font_, sf::Vector2f(btnW, btnH),
        [this]() { if (playerTurn_) onPlayerAttack(); },
        audio_service_);

    btnH += offset;

    btnCons_ = std::make_shared<Button>(
        "Use consumables", *font_, sf::Vector2f(btnW, btnH),
        [this]() { if (playerTurn_) { is_inventory_ = true; useInventory(); }},
        audio_service_);

    //btnH += offset;

    /**btnFlee_ = std::make_shared<Button>(
        "Flee", *font_, sf::Vector2f(btnW, btnH),
        [this]() { finishBattle(false); },
        audio_service_);**/

    btnAttack_->setBackgroundSize(btnAttack_->getBackgroundSize() + sf::Vector2f(25.f, 0.f));
    btnCons_->setBackgroundSize(btnCons_->getBackgroundSize() + sf::Vector2f(25.f, 0.f));

    buff.emplace(0, 0);
    buff.emplace(1, 0);

}

void CombatState::onPlayerAttack() {
    if (buff.at(0))
    {
        enemy_.TakeDamage(player_->GetStats().strength * (1.f + (player_->GetEquippedWeapon() ? player_->GetEquippedWeapon()->effect / 100.f : 0) + buff.at(0) / 100.f));
        buff.at(0) = 0;
    }
    else
    {
        enemy_.TakeDamage(player_->GetStats().strength * (1.f + (player_->GetEquippedWeapon() ? player_->GetEquippedWeapon()->effect / 100.f : 0)));
    }

    audio_service_->playSound(SoundID::Click);

    if (player_->GetEquippedWeapon() && player_->GetEquippedWeapon()->item_sound != -1)
        audio_service_->playSound(static_cast<SoundID>(player_->GetEquippedWeapon()->item_sound));
    else audio_service_->playSound(SoundID::Punch);

    if (enemy_.getSex()) {
        audio_service_->playSound(SoundID::FemaleScream);
    }
    else audio_service_->playSound(SoundID::MaleScream);

    playerTurn_ = false;
}

void CombatState::onEnemyAttack() {
    if (buff.at(1))
    {
        current_hp -= (enemy_.GetDmg() * (1.f - (player_->GetEquippedArmor() ? player_->GetEquippedWeapon()->effect / 100.f : 0) / 100 - buff.at(1) / 100.f));
        buff.at(1) = 0;
    }
    else
    {
        current_hp -= (enemy_.GetDmg() * (1.f - (player_->GetEquippedArmor() ? player_->GetEquippedWeapon()->effect / 100.f : 0)));
    }
    audio_service_->playSound(SoundID::Click);
    playerTurn_ = true;
}

void CombatState::useInventory()
{
    inventory_items_.clear();

    auto inventory = player_->GetInventory();
    float width = render_service_->getRenderWindow().getSize().x;
    float height = render_service_->getRenderWindow().getSize().y;

    float btnW = width / 2.f;
    float btnH = height * 3 / 4 + 30.f;
    float offset = 50.f;

    std::map<int, std::pair<Item, int>> consumable_groups;
    for (const auto& item : inventory)
    {
        if (item.type == "consumable")
        {
            consumable_groups[item.id].first = item;
            consumable_groups[item.id].second++;
        }
    }

    int counter = 0;

    for (auto& [item_id, pair] : consumable_groups)
    {
        const auto& item = pair.first;
        int count = pair.second;

        std::string buttonLabel = item.name + " x" + std::to_string(count);

        std::unique_ptr<Button> new_button(std::make_unique<Button>(
            buttonLabel,
            *font_,
            sf::Vector2f{ btnW, btnH },
            [=, this]() mutable
            {
                if (item.effect_type == "dmg") {
                    buff.at(0) += item.effect;
                }
                else if (item.effect_type == "mp") {
                    buff.at(1) += item.effect;
                }
                else if (item.effect_type == "hp") {
                    int max_hp = player_->GetStats().hp;
                    int heal = static_cast<int>(max_hp * (item.effect / 100.0f));
                    current_hp = std::min(current_hp + heal, max_hp);
                }

                audio_service_->playSound(SoundID::Click);

                player_->RemoveItem(item.id);
                is_inventory_ = false;
                playerTurn_ = false;
                need_rebuild_inventory_ = true;
            },
            audio_service_
        ));

        inventory_items_.emplace(counter++, std::move(new_button));
        btnH += offset;
    }

    std::unique_ptr<Button> close_button(std::make_unique<Button>(
        "Close",
        *font_,
        sf::Vector2f{ btnW, btnH },
        [this]() { is_inventory_ = false; },
        audio_service_
    ));

    inventory_items_.emplace(counter, std::move(close_button));
}



void CombatState::finishBattle(bool victory) {
    if (victory) {
        player_->GainExp(enemy_.getExp());
    }
    onFinish_(victory);
    states_->popState();
}

void CombatState::update(float& deltaTime) {
    audio_service_->update(deltaTime);

    std::ostringstream ps, es;
    ps << "Player HP: " << current_hp;
    es << enemy_.GetName() + " HP: " << enemy_.GetHp();
    txtPlayerHP_->setString(ps.str());
    txtEnemyHP_->setString(es.str());

    if (is_inventory_)
    {
        if (need_rebuild_inventory_) {
            useInventory();
            need_rebuild_inventory_ = false;
        }
            
        for (auto& item : inventory_items_)
        {
            (item.second->update());
        }
    }
    else
    {
        btnAttack_->update();
        btnCons_->update();
        //btnFlee_->update();
    }

    if (!playerTurn_) {
        onEnemyAttack();
    }

    if (enemy_.GetHp() <= 0) finishBattle(true);
    else if (current_hp <= 0) finishBattle(false);
}

void CombatState::render() {
    auto& w = render_service_->getRenderWindow();

    w.draw(*txtPlayerHP_);
    w.draw(*txtEnemyHP_);

    if (is_inventory_)
    {
	    for (auto& item : inventory_items_)
	    {
            w.draw(item.second->getBackground());
            w.draw(item.second->getLabel());
	    }
        return;
    }

	w.draw(btnAttack_->getBackground());
	w.draw(btnAttack_->getLabel());
    w.draw(btnCons_->getBackground());
    w.draw(btnCons_->getLabel());
    //w.draw(btnFlee_->getBackground());
    //w.draw(btnFlee_->getLabel());

}

void CombatState::handleInput(const sf::Event& evt) {
    if (is_inventory_)
    {
        for (auto& item : inventory_items_)
        {
            item.second->handleInput(evt, render_service_->getRenderWindow());
        }
        return;
    }

    btnAttack_->handleInput(evt, render_service_->getRenderWindow());
    btnCons_->handleInput(evt, render_service_->getRenderWindow());
    //btnFlee_->handleInput(evt, render_service_->getRenderWindow());
} 
