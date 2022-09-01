#include "Transition.h"
#include "Game.h"
#include "Resources.h"

Transition *Transition::Instance;
Transition::Transition(GameObject &associated, float duration) : Component(associated), transitionDuration(duration), alpha(255), fadeIn(true), fadeOut(false)
{
    Instance = this;
}

Transition::~Transition()
{
    Instance = nullptr;
}

void Transition::Start()
{
    texture = Resources::GetImage("assets/img/square.png");
    SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture.get(), alpha);
}

void Transition::Update(float dt)
{
    if(fadeIn)
        FadeIn(dt);
    
    if(fadeOut)
        FadeOut(dt);
}

void Transition::Render()
{
    SDL_SetTextureAlphaMod(texture.get(), alpha);
    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture.get(), NULL, NULL, 0, nullptr, SDL_FLIP_NONE);
}

void Transition::FadeIn(float dt)
{
    alpha -= (255 / (transitionDuration / dt));
    if (alpha < 0)
    {
        alpha  = 0;
        fadeIn = false;
    }
}

void Transition::FadeOut(float dt)
{
    alpha += (255 / (transitionDuration / dt));
    if (alpha > 255)
    {
        alpha = 255;
        fadeOut = false;
        Game::GetInstance().GetCurrentState().PopRequested();
        Game::GetInstance().Push(stage);
    }
}

void Transition::StartTransition(State *stage)
{
    this->stage = stage;
    fadeOut = true;
}