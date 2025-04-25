#pragma once
#include <memory>

class IGameState;
class StateService {
public:
    virtual ~StateService() = default;
    virtual void pushState(std::unique_ptr<IGameState> state) = 0;
    virtual void popState() = 0;
};