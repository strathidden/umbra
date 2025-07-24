#include "game/Game.h"
#include "states/MenuState.h"

int main()
{
    Game game;
    game.getStateManager().pushState(std::make_unique<MenuState>());
    game.run();
    return 0;
}
