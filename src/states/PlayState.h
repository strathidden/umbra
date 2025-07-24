#pragma once
#include "GameState.h"
#include "../levels/TileMap.h"
#include "../entities/Player.h"

class PlayState : public GameState
{
public:
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render() override;

private:
    TileMap m_tileMap;
    std::unique_ptr<Player> m_player;
};
