#pragma once
#include "IGameManager.h"
#include "DialogueManager.h"
#include "CharacterManager.h"
#include "UIManager.h"
#include "CombatManager.h"
#include "MissionManager.h"

class DialogueManager;
class Character;
class UIManager;
class CombatManager;
class MissionManager;

class GameManager : public IGameManager
{
private:
	std::unique_ptr<DialogueManager> dialogueManager;
	std::unique_ptr<Character> character;
	std::unique_ptr<UIManager> uiManager;
	std::unique_ptr<CombatManager> combatManager;
	std::unique_ptr<MissionManager> missionManager;
	bool isRunning;
	std::unique_ptr<IGameState> currentState;
public:
	GameManager();
	void ChangeState(std::unique_ptr<IGameState> newState) override;
	void Run() override;
};

