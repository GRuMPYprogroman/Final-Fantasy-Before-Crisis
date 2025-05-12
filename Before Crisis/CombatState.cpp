#include "CombatState.h"
#include <sstream>

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

    btnFlee_ = std::make_shared<Button>(
        "Flee", *font_, sf::Vector2f(300, 360),
        [this]() { finishBattle(false); },
        audio_service_);

    buff.emplace(0, 0);
    buff.emplace(1, 0);

}

void CombatState::onPlayerAttack() {
    if (buff.at(0))
    {
        enemy_.TakeDamage(player_->GetStats().strength * (1 + (player_->GetEquippedWeapon() ? player_->GetEquippedWeapon().value().effect / 100 : 0) + buff.at(0) / 100));
        buff.at(0) = 0;
    }
    else
    {
        enemy_.TakeDamage(player_->GetStats().strength * (1 + (player_->GetEquippedWeapon() ? player_->GetEquippedWeapon().value().effect / 100 : 0) ));
    }
    audio_service_->playSound(SoundID::Click);
    playerTurn_ = false;
}

void CombatState::onEnemyAttack() {
    if (buff.at(1))
    {
        current_hp -= (enemy_.GetDmg() * (1 - (player_->GetEquippedArmor() ? player_->GetEquippedWeapon().value().effect / 100 : 0) / 100 - buff.at(1) / 100 ));
        buff.at(1) = 0;
    }
    else
    {
        current_hp -= (enemy_.GetDmg() * (1 - (player_->GetEquippedArmor() ? player_->GetEquippedWeapon().value().effect / 100 : 0)));
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

    int counter = 0;

    for (auto& item: inventory)
    {
        if (item.type == "consumable")
        {
            const auto item_id = item.id;
            const auto effect = item.effect;
            const auto effect_type = item.effect_type;
            const auto name = item.name;
            const auto local_counter = counter;

            std::unique_ptr<Button> new_button(std::make_unique<Button>(item.name, *font_, sf::Vector2f{ btnW,btnH }, [=, this]()
	        {
                    if (effect_type == "dmg")
                    {
                        buff.at(0) += effect;
                    }
                    else if (effect_type == "mp")
                    {
                        buff.at(1) += effect;
                    }
                    else if (effect_type == "hp")
                    {
                        int max_hp = player_->GetStats().hp;
                        int heal = static_cast<int>(max_hp * (effect / 100.0f));
                        current_hp = std::min(current_hp + heal, max_hp);
                    }

                    audio_service_->playSound(SoundID::Click);
                    player_->RemoveItem(item_id);
                    need_rebuild_inventory_ = true;

	        }, audio_service_ ));

            inventory_items_.emplace(local_counter,std::move(new_button));
            ++counter;
            btnH += offset;
        }
    }
    std::unique_ptr<Button> close_button(std::make_unique<Button>("Close", *font_,sf::Vector2f{btnW,btnH},[&]()
        {
            is_inventory_ = false;
        },
        audio_service_ ));
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
    else if (player_->GetStats().hp <= 0) finishBattle(false);
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
