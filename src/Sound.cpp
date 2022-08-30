#include "Sound.h"
#include "Resources.h"

Sound::Sound(GameObject &associated) : Component(associated), chunk(nullptr) {}

Sound::Sound(std::string file, GameObject &associated) : Sound(associated)
{
    Open(file);
}

void Sound::Update(float dt) {}

void Sound::Play(int times)
{
    channel = Mix_PlayChannel(-1, chunk.get(), times - 1);
}

void Sound::Stop()
{
    if (chunk != nullptr)
    {
        Mix_HaltChannel(channel);
    }
}

void Sound::Open(std::string file)
{
    chunk = Resources::GetSound(file);
    if (chunk == nullptr)
    {
        SDL_Log("Cant load sound: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

Sound::~Sound()
{
}