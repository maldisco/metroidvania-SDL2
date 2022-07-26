#include "Game.h"
#include "Resources.h"
#include "Camera.h"
#include "Sprite.h"

#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"

Sprite::Sprite(GameObject &associated, int frameCount, float frameTime, float secondsToSelfDestruct, int restart) : Component(associated), texture(nullptr),
                                                                                                                    selfDestructCount(), clipScale({1, 1}),
                                                                                                                    frameCount(frameCount), currentFrame(0),
                                                                                                                    timeElapsed(), frameTime(frameTime),
                                                                                                                    secondsToSelfDestruct(secondsToSelfDestruct),
                                                                                                                    restart(restart) {}

Sprite::Sprite(std::string file, GameObject &associated, int frameCount, float frameTime, float secondsToSelfDestruct, int restart) : Component(associated), texture(nullptr),
                                                                                                                                      selfDestructCount(), scale({1, 1}), clipScale({1, 1}),
                                                                                                                                      frameCount(frameCount), currentFrame(0),
                                                                                                                                      timeElapsed(), frameTime(frameTime),
                                                                                                                                      secondsToSelfDestruct(secondsToSelfDestruct),
                                                                                                                                      restart(restart)
{
    Open(file);
}

Sprite::~Sprite() {}

void Sprite::Open(std::string file)
{
    texture = Resources::GetImage(file);

    if (texture == nullptr)
    {
        SDL_Log("Cant load sprite: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height);
    associated.box.w = width / frameCount;
    associated.box.h = height;
    SetClip(0, 0, GetWidth(), GetHeight());
}

void Sprite::Change(std::string file, float frameTime, int frameCount, int restart)
{
    texture = Resources::GetImage(file);
    SetFrameCount(frameCount);
    SetFrameTime(frameTime);
    SetRestart(restart);

    if (texture == nullptr)
    {
        SDL_Log("Cant load sprite: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height);
    Rect newBox = {associated.box.x, associated.box.y, width / frameCount, height};
    newBox.Centered(associated.box.Center());
    associated.box = newBox;
    SetClip(0, 0, GetWidth(), GetHeight());
}

void Sprite::SetClip(int x, int y, int w, int h)
{
    clipRect.h = h;
    clipRect.w = w;
    clipRect.x = x;
    clipRect.y = y;
}

void Sprite::Update(float dt)
{
    timeElapsed.Update(dt);
    if (timeElapsed.Get() > frameTime)
    {
        currentFrame++;
        if (currentFrame >= frameCount)
        {
            currentFrame = restart;
        }
        SetClip((currentFrame)*GetWidth(), 0, GetWidth() * clipScale.x, GetHeight());
        timeElapsed.Restart();
    }

    if (secondsToSelfDestruct > 0)
    {
        selfDestructCount.Update(dt);
        if (selfDestructCount.Get() > secondsToSelfDestruct)
        {
            associated.RequestDelete();
        }
    }
}

void Sprite::Render(int x, int y)
{
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = clipRect.w * scale.x;
    dstrect.h = clipRect.h * scale.y;

    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture.get(), &clipRect, &dstrect, associated.angleDeg - Camera::virtualAngle,
                     nullptr, associated.direction >= 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

void Sprite::Render()
{
    Render(associated.box.x - Camera::virtualPos.x, associated.box.y - Camera::virtualPos.y);
}

void Sprite::SetScale(float scaleX, float scaleY)
{
    scale.x = scaleX;
    scale.y = scaleY;

    associated.box.w = clipRect.w * scale.x;
    associated.box.h = clipRect.h * scale.y;
}

void Sprite::SetClipScale(float clipScaleX, float clipScaleY)
{
    clipScale.x = clipScaleX;
    clipScale.y = clipScaleY;
}

void Sprite::SetFrame(int frame)
{
    if (frame < frameCount)
    {
        currentFrame = frame;
        SetClip((currentFrame)*GetWidth(), 0, GetWidth(), GetHeight());
    }
}

void Sprite::SetFrameCount(int frameCount)
{
    this->frameCount = frameCount;
    this->currentFrame = 0;
}

void Sprite::SetFrameTime(float frameTime)
{
    this->frameTime = frameTime;
}

void Sprite::SetRestart(int restart)
{
    this->restart = restart;
}

void Sprite::SetTransparency(float alpha)
{
    SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture.get(), alpha*255);
}

Vec2 Sprite::GetScale()
{ 
    return scale; 
}

Vec2 Sprite::GetClipScale() 
{ 
    return clipScale; 
}

int Sprite::GetWidth()
{
    return width / frameCount;
}

int Sprite::GetHeight()
{
    return height;
}

int Sprite::GetCurrentFrame()
{
    return currentFrame;
}

int Sprite::GetFrameCount()
{
    return frameCount;
}

SDL_Texture *Sprite::GetTexture()
{
    return texture.get();
}

bool Sprite::IsOpen()
{
    return texture != nullptr;
}
