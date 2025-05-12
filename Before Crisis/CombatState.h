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
private:
	std::shared_ptr<RenderService> render_service_;
	std::shared_ptr<AudioService> audio_service_;
	std::shared_ptr<StateService> states_;
	std::shared_ptr<Character> player_;
	Monster enemy_;
	std::function<void(bool)> onFinish_;

	std::unique_ptr<sf::Text> txtPlayerHP_;
	std::unique_ptr<sf::Text> txtEnemyHP_;
	std::shared_ptr<sf::Font> font_;
	std::shared_ptr<Button> btnAttack_;
	std::shared_ptr<Button> btnFlee_;
	std::shared_ptr<Button> btnCons_;

	int& current_hp;

	bool is_inventory_ = false;
	std::map<int,std::unique_ptr<Button>> inventory_items_;
	std::map<int, int> buff; // 0 - dmg, 1 - mp

	bool need_rebuild_inventory_ = false;

	bool playerTurn_ = true;

	void onPlayerAttack();
	void onEnemyAttack();
	void useInventory();
	void finishBattle(bool victory);
public:
	CombatState(std::shared_ptr<RenderService> renderService,
		std::shared_ptr<AudioService> audioService,
		std::shared_ptr<StateService> stateService,
		std::shared_ptr<Character> player,
		Monster enemy,
		std::function<void(bool)> onFinish,
		int& currentHp);

	void update(float& deltaTime) override;
	void render() override;
	void handleInput(const sf::Event& evt) override;
};
