#pragma once
#include "IGameState.h"
#include "DialogueManager.h"
#include "CharacterManager.h"
#include "CombatManager.h"
#include "MissionManager.h"
#include "MenuState.h"
#include "Renderer.h"
#include <memory>
#include "GameplayState.h"
#include "CombatState.h"

class DialogueManager;
class Character;
class CombatManager;
class MissionManager;
class Renderer;
class MenuState;
class GameplayState;
class CombatState;

class GameManager
{
private:
	//std::unique_ptr<IDialogueManager> dialogueManager;
	std::unique_ptr<Character> character;
	//std::unique_ptr<ICombatManager> combatManager;
	std::unique_ptr<IMissionManager> missionManager;
	std::unique_ptr<IGameState> currentState;
	std::shared_ptr<Renderer> renderer;
	bool isRunning;
public:
	GameManager();
	void ChangeState(std::unique_ptr<IGameState> newState);
	void Run();
};

