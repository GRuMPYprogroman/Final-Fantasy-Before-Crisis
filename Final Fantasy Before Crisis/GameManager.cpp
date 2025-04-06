#include "GameManager.h"


GameManager::GameManager()
	: dialogueManager(std::make_unique<DialogueManager>()),
    character(std::make_unique<Character>()),
    uiManager(std::make_unique<UIManager>()),
    combatManager(std::make_unique<CombatManager>()),
    missionManager(std::make_unique<MissionManager>()),
    isRunning(true)
{}

void GameManager::ChangeState(std::unique_ptr<IGameState> newState)
{
	if (currentState) currentState->Exit(); 
	currentState = std::move(newState);
	currentState->Enter();
}

void GameManager::Run()
{
	while (isRunning)
	{
		uiManager->DisplayText("Welcome to Before Crisis!");
		uiManager->DisplayText("1. Start Game 2. Exit");
		int choice = uiManager->GetChoice(1, 2);
		if (choice == 2) isRunning = false;
	}
}