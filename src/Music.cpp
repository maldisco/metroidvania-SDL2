#include "Music.h"
#include "Resources.h"

#define INCLUDE_SDL
#include "SDL_include.h"

Music::Music() : music(nullptr){}

Music::Music(std::string file) : Music(){
    Open(file);
}

void Music::Play(int times){
    if (Mix_PlayMusic(music.get(), times) == -1){
        SDL_Log("Cant play music: %s", SDL_GetError());
        
    }
}

void Music::Stop(int msToStop){
    Mix_FadeOutMusic(msToStop);
}

void Music::Open(std::string file){
    music = Resources::GetMusic(file);

    if (music == nullptr){
        SDL_Log("Cant load music: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

bool Music::IsOpen(){
    return music != nullptr;
}

Music::~Music(){
}