#pragma once
#include "IGameState.h"

class CombatState : public IGameState
{
public:
    void Enter() override;
	void Update() override;
	void Exit() override;
};

