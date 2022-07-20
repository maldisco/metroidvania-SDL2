#include "Minion.h"
#include "Bullet.h"
#include "Collider.h"
#include "Game.h"
#include <cmath>

Minion::Minion(GameObject& associated, std::weak_ptr<GameObject> alienCenter, float arcOffsetDeg) : Component(associated), alienCenter(alienCenter), arc(arcOffsetDeg){
    float scale = 1 + (float)(rand()%50)/100;
    Sprite* minionSprite = new Sprite("assets/img/minion.png", associated, 1, 1.0);
    minionSprite->SetScaleX(sqrt(scale), sqrt(scale));

    associated.AddComponent(minionSprite);
    associated.AddComponent(new Collider(associated));

    Vec2 origin =  Vec2::Rotate(Vec2(100, 0), arc) + alienCenter.lock().get()->box.Center();
    associated.box.Centered(origin);
}

void Minion::Update(float dt){
    if(alienCenter.lock() == nullptr){
        associated.RequestDelete();
        return;
    }
    // increase arc
    arc += ARC;
    associated.angleDeg = arc*180/PI;

    Vec2 origin = Vec2::Rotate(Vec2(100, 0), arc) + alienCenter.lock().get()->box.Center();
    associated.box.Centered(origin);
}

void Minion::Shoot(Vec2 target){
    Vec2 dir = target - associated.box.Center();
    float angle = dir.SlopeX();

    GameObject* bulletObject = new GameObject();
    // Bullet(associated, angle, speed, damage, maxDistance, sprite, frameTime, frameCount)
    bulletObject->AddComponent(new Bullet(*bulletObject, angle, 500, 10, 500, "assets/img/minionbullet2.png", 0.05, 3, true));
    bulletObject->box.x = associated.box.x;
    bulletObject->box.y = associated.box.y;
    bulletObject->angleDeg = angle*180/PI;
    Game::GetInstance().GetCurrentState().AddObject(bulletObject);
}

void Minion::Render(){}

void Minion::NotifyCollision(GameObject& other){}

bool Minion::Is(std::string type){
    if(type.compare("Minion") == 0){
        return true;
    }

    return false;
}
