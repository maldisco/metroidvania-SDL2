#include "Game.h"
#include "MenuState.h"

int main(int argc, char **argv)
{
    Game game = Game::GetInstance();
    State *initialState = new MenuState();
    game.Push(initialState);
    game.Run();

    return 0;
}