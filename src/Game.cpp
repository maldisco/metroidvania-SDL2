#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "Game.h"
#include "InputManager.h"
#include "Resources.h"
#include <ctime>
#include <iostream>

Game* Game::instance;
std::stack<std::unique_ptr<State>> Game::stateStack;
State* Game::storedState;
Game& Game::GetInstance(){
    if (instance == nullptr)
    {
        new Game("Filipe de Sousa Fernandes - 202065879", 1024, 600);
    }

    return *instance;
}

Game::Game(std::string title, int width, int height) : frameStart(0), dt(0){
    instance = this;

    // Initialization of basic SDL functionalities 
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        SDL_Log("Cant initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0)
    {
        SDL_Log("Cant initialize IMG: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) == 0)
    {
        SDL_Log("Cant initialize MIX: %s", SDL_GetError());
    }

    if (TTF_Init() != 0)
    {
        SDL_Log("Cant initialize TTF: %s", SDL_GetError());
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0)
    {
        SDL_Log("Cant initialize OpenAudio: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    Mix_AllocateChannels(32);

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if(window == nullptr){
        SDL_Log("Cant initialize window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr){
        SDL_Log("Cant initialize renderer: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    frameStart = SDL_GetTicks();
    storedState = nullptr;

    srand(time(NULL));
}

Game::~Game(){   
    if(storedState != nullptr){
        delete storedState;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

State& Game::GetCurrentState(){
    State* cState = stateStack.top().get();
    return *cState;
}

void Game::Push(State* state){
    storedState = state;
}

SDL_Renderer* Game::GetRenderer(){
    return renderer;
}

void Game::Run(){
    stateStack.emplace(storedState);
    stateStack.top()->Start();
    storedState = nullptr;

    while (!(stateStack.empty()) and !(stateStack.top()->QuitRequested())){
        if(stateStack.top()->PopRequested()){
            stateStack.pop();
            Resources::ClearImages();

            if(!stateStack.empty()){
                stateStack.top()->Resume();
            }
        }

        if(storedState != nullptr){
            stateStack.top()->Pause();
            stateStack.emplace(storedState);
            stateStack.top()->Start();
            storedState = nullptr;
        }

        CalculateDeltaTime();
        InputManager::GetInstance().Update();
        stateStack.top()->Update(GetDeltaTime());
        stateStack.top()->Render();
        SDL_RenderPresent(renderer);
        SDL_Delay(33);
    }
    
    while(!stateStack.empty()) {stateStack.pop();}
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();
}

void Game::CalculateDeltaTime(){
    dt = (SDL_GetTicks() - frameStart);
    frameStart = SDL_GetTicks();
}

float Game::GetDeltaTime(){ return dt/1000; }

