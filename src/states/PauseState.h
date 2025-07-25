#pragma once
#include "GameState.h"
#include "../ui/Button.h"
#include <memory>

class PauseState : public GameState
{
public:
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render() override;

private:
    std::unique_ptr<Button> m_resumeButton;
    std::unique_ptr<Button> m_menuButton;
};
