#pragma once
#include <vector>
#include <memory>
#include "IGameState.h"
#include "StateService.h"

class StateStack : public StateService
{
private:
    std::vector<std::unique_ptr<IGameState>> states;
public:
    void pushState(std::unique_ptr<IGameState> state) override;
    void popState() override;
    bool empty() const;
    const std::vector<std::unique_ptr<IGameState>>& getStates() const;
    IGameState* topState() const;
};