#pragma once

class GameState
{
public:
    virtual ~GameState() = default;

    virtual void enter() {}
    virtual void exit() {}
    
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
};
