#pragma once

class IGameState {
public:
    virtual ~IGameState() = default;
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;
};