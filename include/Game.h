#ifndef GAME_H
#define GAME_H

#define INCLUDE_SDL
#include "SDL_include.h"

#include "State.h"

#include <stack>

/**
 * @brief Class responsible for running main game loop
 */
class Game
{
private:
    /**
     * @brief Instantiate a Game object and initialize SDL libraries
     *
     * @param title
     * @param width
     * @param height
     */
    Game(std::string title, int width, int height);

    /**
     * @brief Unique instance of Game (Singleton)
     */
    static Game *instance;

    /**
     * @brief Game window
     */
    SDL_Window *window;

    /**
     * @brief Responsible for filling the window
     */
    SDL_Renderer *renderer;

    /**
     * @brief Object that controls the state of the game
     */
    static State *storedState;
    static std::stack<std::unique_ptr<State>> stateStack;

    /**
     * @brief time when frame started
     */
    int frameStart;

    /**
     * @brief delta time (time passed)
     */
    float dt;

    void CalculateDeltaTime();

public:
    /**
     * @brief Destroy the state object and quit all SDL functionalities
     */
    ~Game();

    /**
     * @brief The game loop
     */
    void Run();

    /**
     * @brief Get the Renderer object
     *
     * @return SDL_Renderer*
     */
    SDL_Renderer *GetRenderer();

    /**
     * @brief Get the State object
     *
     * @return State&
     */
    State &GetCurrentState();

    /**
     * @brief Push a state to the stack
     *
     * @param state
     */
    void Push(State *state);

    /**
     * @brief Get the only instance of Game class
     *
     * @return Game&
     */
    static Game &GetInstance();

    /**
     * @brief return dt
     */
    float GetDeltaTime();
};
#endif