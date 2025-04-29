#include "StateStack.h"

void StateStack::pushState(std::unique_ptr<IGameState> state) { pendingChanges_.push_back({ PendingAction::Push, std::move(state) }); }
void StateStack::popState() { pendingChanges_.push_back({ PendingAction::Pop, nullptr }); }
bool StateStack::empty() const { return states.empty(); }
const std::vector<std::unique_ptr<IGameState>>& StateStack::getStates() const { return states; }
IGameState* StateStack::topState() const { return states.empty() ? nullptr : states.back().get(); }

void StateStack::applyPendingChanges()
{
	for (auto& change : pendingChanges_)
	{
		switch (change.action)
		{
		case PendingAction::Push:
		{
			states.push_back(std::move(change.newState));
			break;
		}
		case PendingAction::Pop:
		{
			states.pop_back();
			break;
		}

		}
	}
	pendingChanges_.clear();
}
