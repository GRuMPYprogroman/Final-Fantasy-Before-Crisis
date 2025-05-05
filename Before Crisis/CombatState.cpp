#include "CombatState.h"
#include <sstream>

CombatState::CombatState(
    std::shared_ptr<RenderService> renderService,
    std::shared_ptr<AudioService>  audioService,
    std::shared_ptr<StateService>  stateService,
    std::shared_ptr<Character>     player,
    MonsterType                    type,
    std::function<void(bool)>      onFinish)
    : render_(renderService)
    , audio_(audioService)
    , states_(stateService)
    , player_(player)
    , onFinish_(std::move(onFinish))
{
	txtPlayerHP_ = std::make_unique<sf::Text>(*font_);
	txtEnemyHP_ = std::make_unique<sf::Text>(*font_);

    font_ = std::make_unique<sf::Font>(render_->getDefaultFont());
    txtPlayerHP_->setCharacterSize(20);
    txtPlayerHP_->setPosition({ 20, 20 });
    txtEnemyHP_->setCharacterSize(20);
    txtEnemyHP_->setPosition({ 20, 60 });

    spawnEnemy(type);

    btnAttack_ = std::make_shared<Button>(
        "Attack", *font_, sf::Vector2f(300, 300),
        [this]() { if (playerTurn_) onPlayerAttack(); },
        audio_);
    btnFlee_ = std::make_shared<Button>(
        "Flee", *font_, sf::Vector2f(300, 360),
        [this]() { finishBattle(false); },
        audio_);

}

void CombatState::spawnEnemy(MonsterType type) {
    enemy_ = std::make_unique<Monster>(type, 0, 0);
    switch (type)
	{
    //case MonsterType::Goblin: enemy_->health = 30; enemy_->attack = 5; break;
    //case MonsterType::Wolf:   enemy_->health = 40; enemy_->attack = 7; break;
    //default:                   enemy_->health = 20; enemy_->attack = 3; break;
    }
}

void CombatState::onPlayerAttack() {
    enemy_->TakeDamage(player_->GetStats().strength);
    audio_->playSound(SoundID::Click);
    playerTurn_ = false;
}

void CombatState::onEnemyAttack() {
    player_->TakeDamage(enemy_->GetAttack());
    audio_->playSound(SoundID::Click);
    playerTurn_ = true;
}

void CombatState::finishBattle(bool victory) {
    if (victory) {
        player_->GainExp(enemy_->GetAttack() * 10);
    }
    onFinish_(victory);
    states_->popState();
}

void CombatState::update(float& deltaTime) {
    std::ostringstream ps, es;
    ps << "Player HP: " << player_->GetStats().hp;
    es << "Enemy HP: " << enemy_->GetHealth();
    txtPlayerHP_->setString(ps.str());
    txtEnemyHP_->setString(es.str());

    btnAttack_->update();
    btnFlee_->update();

    if (!playerTurn_) {
        onEnemyAttack();
    }

    if (enemy_->GetHealth() <= 0) finishBattle(true);
    else if (player_->GetStats().hp <= 0) finishBattle(false);
}

void CombatState::render() {
    auto& w = render_->getRenderWindow();
    w.draw(*txtPlayerHP_);
    w.draw(*txtEnemyHP_);
    w.draw(btnAttack_->getBackground());
    w.draw(btnAttack_->getLabel());
    w.draw(btnFlee_->getBackground());
    w.draw(btnFlee_->getLabel());
}

void CombatState::handleInput(const sf::Event& evt) {
    btnAttack_->handleInput(evt, render_->getRenderWindow());
    btnFlee_->handleInput(evt, render_->getRenderWindow());
}
