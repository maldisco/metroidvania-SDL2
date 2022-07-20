#include "PenguinBody.h"
#include "PenguinCannon.h"
#include "Sprite.h"
#include "Collider.h"
#include "Game.h"
#include "Camera.h"
#include "Bullet.h"
#include "Timer.h"
#include "Sound.h"
#include "InputManager.h"

PenguinBody* PenguinBody::player;
PenguinBody::PenguinBody(GameObject& associated) : Component(associated), speed({1,0}), linearSpeed(0), angle(0), hp(100){
    player = this;
    associated.AddComponent(new Sprite("assets/img/penguin.png", associated, 1, 1.0));
    associated.AddComponent(new Collider(associated));
}

PenguinBody::~PenguinBody(){
    player = nullptr;
}

void PenguinBody::Start(){
    GameObject* go = new GameObject();
    go->AddComponent(new PenguinCannon(*go, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated)));
    this->pcannon = Game::GetInstance().GetCurrentState().AddObject(go);
}

void PenguinBody::Update(float dt){
    if(InputManager::GetInstance().IsKeyDown(W_KEY)){
        linearSpeed = std::min(linearSpeed+5, float(300));
    } else if (InputManager::GetInstance().IsKeyDown(S_KEY)){
        linearSpeed = std::max(linearSpeed-5, float(-300));
    } else {
        linearSpeed = std::max(linearSpeed-5, float(0));
    }
    if (InputManager::GetInstance().IsKeyDown(A_KEY)){
        speed = Vec2::Rotate(speed, -PI/64);
    } 
    if (InputManager::GetInstance().IsKeyDown(D_KEY)){
        speed = Vec2::Rotate(speed, PI/64);
    }   

    Rect newPos = associated.box + (speed*(linearSpeed*dt));
    if(newPos.x > 0 and newPos.x < 1408 and newPos.y > 0 and newPos.y < 1280){
        associated.box = newPos;
    }
    associated.angleDeg = speed.SlopeX()*180/PI;
}

void PenguinBody::NotifyCollision(GameObject& other){
    if(other.GetComponent("Bullet") != nullptr){
        Bullet* bullet = (Bullet*)other.GetComponent("Bullet");
        if(bullet->targetsPlayer){
            this->hp -= 5;
            if(this->hp <= 0){
                Camera::Unfollow();
                associated.RequestDelete();
                pcannon.lock()->RequestDelete();

                GameObject* penguimDeath = new GameObject();
	            penguimDeath->AddComponent(new Sprite("assets/img/penguindeath.png", *penguimDeath, 5, 0.1f, 0.5f));
                Sound* sound = new Sound("assets/audio/boom.wav", *penguimDeath);
                sound->Play();
                penguimDeath->AddComponent(sound);
	            penguimDeath->box.Centered(associated.box.Center());
	            Game::GetInstance().GetCurrentState().AddObject(penguimDeath);
            }
        }
    }
}

void PenguinBody::Render(){}

bool PenguinBody::Is(std::string type){
    if(type.compare("Penguin body") == 0){
        return true;
    }

    return false;
}