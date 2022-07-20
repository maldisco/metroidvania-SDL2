#include "Android.h"
#include "Sprite.h"
#include "Collider.h"
#include "Game.h"
#include "Camera.h"
#include "Bullet.h"
#include "Timer.h"
#include "Sound.h"
#include "InputManager.h"

Android* Android::player;
Android::Android(GameObject& associated) : Component(associated), speed({1,0}), linearSpeed(0), angle(0), hp(100){
    player = this;
    associated.AddComponent(new Sprite("assets/img/2bstand.png", associated, 3, 0.2f));
    associated.AddComponent(new Collider(associated));
}

Android::~Android(){
    player = nullptr;
}

void Android::Start(){
    
}

void Android::Update(float dt){
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

    associated.box = associated.box + (speed*(linearSpeed*dt));
    associated.angleDeg = speed.SlopeX()*180/PI;
}

void Android::NotifyCollision(GameObject& other){
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

void Android::Render(){}

bool Android::Is(std::string type){
    if(type.compare("Android") == 0){
        return true;
    }

    return false;
}