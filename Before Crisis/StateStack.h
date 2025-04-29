#pragma once
#include <vector>
#include <memory>
#include "IGameState.h"
#include "StateService.h"


class StateStack : public StateService
{
private:
	enum class PendingAction
	{
		Push,
		Pop
	};

    struct PendingChange {
        PendingAction action;
        std::unique_ptr<IGameState> newState;
    };
    std::vector<PendingChange> pendingChanges_;

    std::vector<std::unique_ptr<IGameState>> states;
public:
    void pushState(std::unique_ptr<IGameState> state) override;
    void popState() override;
    bool empty() const;
    const std::vector<std::unique_ptr<IGameState>>& getStates() const;
    IGameState* topState() const;
    void applyPendingChanges();
};