#include "Alien.h"
#include "Minion.h"
#include "Sprite.h"
#include "Collider.h"
#include "InputManager.h"
#include "Camera.h"
#include "Sound.h"
#include "Game.h"
#include "Bullet.h"


int Alien::alienCount = 0;
Alien::Alien(GameObject& associated, float timeOffset) : Component(associated), speed({100, 100}), hp(100), state(RESTING), restTimer(), timeOffset(timeOffset){
    associated.AddComponent(new Sprite("assets/img/alien.png", associated, 1, 1.0));
    associated.AddComponent(new Collider(associated));
    Alien::alienCount++;
}

Alien::~Alien(){
    minionArray.clear();
    Alien::alienCount--;
}

void Alien::Start(){
    GameObject* minion = new GameObject();
    minion->AddComponent(new Minion(*minion, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated)));
    minionArray.push_back(Game::GetInstance().GetCurrentState().AddObject(minion));

    GameObject* minion2 = new GameObject();
    minion2->AddComponent(new Minion(*minion2, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated), PI/2));
    minionArray.push_back(Game::GetInstance().GetCurrentState().AddObject(minion2));

    GameObject* minion3 = new GameObject();
    minion3->AddComponent(new Minion(*minion3, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated), PI));
    minionArray.push_back(Game::GetInstance().GetCurrentState().AddObject(minion3));

    GameObject* minion4 = new GameObject();
    minion4->AddComponent(new Minion(*minion4, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated), 3*PI/2));
    minionArray.push_back(Game::GetInstance().GetCurrentState().AddObject(minion4));      
}

void Alien::Update(float dt){
    // rotate alien sprite
    associated.angleDeg -= (ARC/2)*180/PI;

    if(state == AlienState::RESTING){
        if(restTimer.Get() >= 0.5f){
            destination = Camera::pos + Vec2(CAMERA_WIDTH/2, CAMERA_HEIGHT/2);
            Vec2 direction = destination - associated.box.Center();
            speed = Vec2::Rotate(speed, Vec2::Slope(direction, speed));
            state = MOVING;
        } else {
            restTimer.Update(dt + timeOffset);
        }
    } else {
        if(Vec2::Distance(associated.box.Center(), destination) < 5.0f){
            destination = Camera::pos + Vec2(CAMERA_WIDTH/2, CAMERA_HEIGHT/2);
            // search for minion closer to mouse click
            int closerDistance = INT_MAX;
            std::weak_ptr<GameObject> closerMinion;
            for(auto minion : minionArray){
                float distance = Vec2::Distance(minion.lock()->box.Center(), destination);
                if(distance < closerDistance){
                    closerDistance = distance;
                    closerMinion = minion;
                }
            }
            Minion* minion = (Minion*)(closerMinion.lock()->GetComponent("Minion"));
            minion->Shoot(destination);

            state = RESTING;
            restTimer.Restart();
        } else {
            associated.box = associated.box + speed*dt;
        }
    }

}

void Alien::NotifyCollision(GameObject& other){
    if(other.GetComponent("Bullet") != nullptr){
        Bullet* bullet = (Bullet*)other.GetComponent("Bullet");
        if(not bullet->targetsPlayer){
            this->hp -= bullet->GetDamage();
            if(this->hp <= 0){
                associated.RequestDelete();

                GameObject* alienDeath = new GameObject();
	            alienDeath->AddComponent(new Sprite("assets/img/aliendeath.png", *alienDeath, 4, 0.1f, 0.4f));
                Sound* sound = new Sound("assets/audio/boom.wav", *alienDeath);
                sound->Play();
                alienDeath->AddComponent(sound);
	            alienDeath->box.Centered(associated.box.Center());
	            Game::GetInstance().GetCurrentState().AddObject(alienDeath);
            }
        }
    }
}

void Alien::Render(){}

bool Alien::Is(std::string type){
    if(type.compare("Alien") == 0){
        return true;
    }

    return false;
}