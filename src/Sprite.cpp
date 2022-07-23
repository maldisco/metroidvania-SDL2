#include "Game.h"
#include "Resources.h"
#include "Camera.h"
#include "Sprite.h"

#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"

Sprite::Sprite(GameObject& associated, int frameCount, float frameTime, float secondsToSelfDestruct, int restart) : Component(associated), texture(nullptr), selfDestructCount(),
 frameCount(frameCount), currentFrame(0), timeElapsed(0), frameTime(frameTime), secondsToSelfDestruct(secondsToSelfDestruct), dir(0), restart(restart) {}

Sprite::Sprite(std::string file, GameObject& associated, int frameCount, float frameTime, float secondsToSelfDestruct, int restart) : Component(associated), texture(nullptr),
 selfDestructCount(), scale({1, 1}), frameCount(frameCount), currentFrame(0), timeElapsed(0), frameTime(frameTime), secondsToSelfDestruct(secondsToSelfDestruct), dir(0),
 restart(restart) {
    Open(file);
}

Sprite::~Sprite(){}

void Sprite::Open(std::string file){
    texture = Resources::GetImage(file);

    if (texture == nullptr){
        SDL_Log("Cant load sprite: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height);
    associated.box.w = width/frameCount;
    associated.box.h = height;
    SetClip(0, 0, GetWidth(), GetHeight());
}

void Sprite::Change(std::string file, float frameTime, int frameCount, int restart){
    texture = Resources::GetImage(file);
    SetFrameCount(frameCount);
    SetFrameTime(frameTime);
    SetRestart(restart);

    if (texture == nullptr){
        SDL_Log("Cant load sprite: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height);
    Rect newBox = {associated.box.x, associated.box.y, width/frameCount, height};
    newBox.Centered(associated.box.Center());
    associated.box = newBox;
    SetClip(0, 0, GetWidth(), GetHeight());
}

void Sprite::SetClip(int x, int y, int w, int h){
    clipRect.h = h;
    clipRect.w = w;
    clipRect.x = x;
    clipRect.y = y;
}

void Sprite::Update(float dt){
    timeElapsed += dt;
    if(timeElapsed > frameTime){
        currentFrame++;
        timeElapsed = 0;
        if(currentFrame >= frameCount){
            currentFrame = restart;
        }
        SetClip((currentFrame)*GetWidth(), 0, GetWidth(), GetHeight());
    }

    if(secondsToSelfDestruct > 0){
        selfDestructCount.Update(dt);
        if(selfDestructCount.Get()>secondsToSelfDestruct){
            associated.RequestDelete();
        }
    }
}

bool Sprite::Is(std::string type){
    if(type.compare("Sprite") == 0){
        return true;
    }

    return false;
}

void Sprite::Render(int x, int y){
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = clipRect.w*scale.x;
    dstrect.h = clipRect.h*scale.y;

    if(dir >= 0){
        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture.get(), &clipRect, &dstrect, associated.angleDeg, nullptr, SDL_FLIP_NONE);
    } else {
        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture.get(), &clipRect, &dstrect, associated.angleDeg, nullptr, SDL_FLIP_HORIZONTAL);
    }
}

void Sprite::Render(){
    Render(associated.box.x - Camera::pos.x, associated.box.y - Camera::pos.y);
}

void Sprite::NotifyCollision(GameObject& other){}

void Sprite::SetScaleX(float scaleX, float scaleY){
    // % of increase/decrease in size rate
    float widthChangeRate = scaleX - scale.x;
    float heightChangeRate = scaleY - scale.y;

    // absolute value of increase/decrease 
    float widthChange = associated.box.w*widthChangeRate;
    float heightChange = associated.box.h*heightChangeRate;

    // rescale associated box
    associated.box.w += widthChange;
    width = associated.box.w;
    associated.box.h += heightChange;
    height = associated.box.h;

    scale.x = scaleX;
    scale.y = scaleY;
}

void Sprite::SetFrame(int frame){
    if(frame < frameCount){
        currentFrame = frame;
        SetClip((currentFrame)*GetWidth(), 0, GetWidth(), GetHeight());
    }
}

void Sprite::SetDir(int dir){ this->dir = dir; }

void Sprite::SetFrameCount(int frameCount){
    this->frameCount = frameCount;
    this->currentFrame = 0;
}

void Sprite::SetFrameTime(float frameTime){
    this->frameTime = frameTime;
}

void Sprite::SetRestart(int restart){
    this->restart = restart; 
}

Vec2 Sprite::GetScale(){ return scale; }

int Sprite::GetWidth(){
    return width/frameCount;
}

int Sprite::GetHeight(){
    return height;
}

bool Sprite::IsOpen(){
    return texture != nullptr;
}
