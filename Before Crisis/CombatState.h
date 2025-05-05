#pragma once
#include "IGameState.h"
#include "RenderService.h"
#include "AudioService.h"
#include "StateService.h"
#include "CharacterManager.h"
#include "Monster.h"
#include "Button.h"
#include <memory>
#include <functional>

class CombatState : public IGameState {
public:
	CombatState(std::shared_ptr<RenderService> renderService,
		std::shared_ptr<AudioService> audioService,
		std::shared_ptr<StateService> stateService,
		std::shared_ptr<Character> player,
		MonsterType type,
		std::function<void(bool)> onFinish);

	void update(float& deltaTime) override;
	void render() override;
	void handleInput(const sf::Event& evt) override;

private:
	std::shared_ptr<RenderService> render_;
	std::shared_ptr<AudioService>  audio_;
	std::shared_ptr<StateService>  states_;
	std::shared_ptr<Character>     player_;
	std::unique_ptr<Monster>       enemy_;
	std::function<void(bool)>      onFinish_;

	std::unique_ptr<sf::Text>      txtPlayerHP_;
	std::unique_ptr<sf::Text>      txtEnemyHP_;
	std::shared_ptr<sf::Font>      font_;
	std::shared_ptr<Button>        btnAttack_;
	std::shared_ptr<Button>        btnFlee_;

	bool playerTurn_ = true;

	void spawnEnemy(MonsterType type);
	void onPlayerAttack();
	void onEnemyAttack();
	void finishBattle(bool victory);
};
