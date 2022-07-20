#include "Resources.h"
#include "Game.h"
#include <string>

std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> Resources::imageTable;
std::unordered_map<std::string, std::shared_ptr<Mix_Music>> Resources::musicTable;
std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> Resources::soundTable;
std::unordered_map<std::string, std::shared_ptr<TTF_Font>> Resources::fontTable;

std::shared_ptr<SDL_Texture> Resources::GetImage(std::string file){
    if(imageTable.find(file) != imageTable.end()){
        return imageTable[file];
    }

    SDL_Texture* texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    imageTable[file] = std::shared_ptr<SDL_Texture>(texture, [](SDL_Texture* ptr) {SDL_DestroyTexture(ptr);});
    return imageTable[file];
}

void Resources::ClearImages(){
    for(auto& image : imageTable){
        if(image.second.unique()){
            imageTable.erase(image.first);
        }
    }
}

std::shared_ptr<Mix_Music> Resources::GetMusic(std::string file){
    if(musicTable.find(file) != musicTable.end()){
        return musicTable[file];
    }

    Mix_Music* music = Mix_LoadMUS(file.c_str());
    musicTable[file] = std::shared_ptr<Mix_Music>(music, [](Mix_Music* ptr) {Mix_FreeMusic(ptr);});
    return musicTable[file];
}

void Resources::ClearMusics(){
    for(auto& music : musicTable){
        if(music.second.unique()){
            musicTable.erase(music.first);
        }
    }
}

std::shared_ptr<Mix_Chunk> Resources::GetSound(std::string file){
    if(soundTable.find(file) != soundTable.end()){
        return soundTable[file];
    }

    Mix_Chunk* sound = Mix_LoadWAV(file.c_str());
    soundTable[file] = std::shared_ptr<Mix_Chunk>(sound, [](Mix_Chunk* ptr) {Mix_FreeChunk(ptr);});
    return soundTable[file];
}

void Resources::ClearSounds(){
    for(auto& sound : soundTable){
        if(sound.second.unique()){
            soundTable.erase(sound.first);
        }
    }
}

std::shared_ptr<TTF_Font> Resources::GetFont(std::string file, int size){
    std::string key = file + std::to_string(size);
    if(fontTable.find(key) != fontTable.end()){
        return fontTable[key];
    }

    TTF_Font* font = TTF_OpenFont(file.c_str(), size);
    fontTable[key] = std::shared_ptr<TTF_Font>(font, [](TTF_Font* ptr) {TTF_CloseFont(ptr);});
    return fontTable[key];
}

void Resources::ClearFonts(){
    for(auto& font : fontTable){
        if(font.second.unique()){
            fontTable.erase(font.first);
        }
    }
}