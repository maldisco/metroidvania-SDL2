#include "Game.h"
#include "TitleState.h"

int main(int argc, char **argv)
{   
    Game game = Game::GetInstance();
    State* initialState = new TitleState();
    game.Push(initialState);
    game.Run();

    return 0;
}