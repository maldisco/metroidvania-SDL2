#include "PenguinCannon.h"
#include "Sprite.h"
#include "InputManager.h"
#include "Bullet.h"
#include "Collider.h"
#include "Game.h"
#include "Camera.h"
#include "Timer.h"

PenguinCannon::PenguinCannon(GameObject& associated, std::weak_ptr<GameObject> penguinBody) : Component(associated), pbody(penguinBody), angle(0){
    associated.AddComponent(new Sprite("assets/img/cubngun.png", associated));
    associated.AddComponent(new Collider(associated));
}

void PenguinCannon::Update(float dt){
    if(pbody.lock()->IsDead()){
        associated.RequestDelete();
    }
    // Components and useful objects
    Vec2 mouseCoord = {InputManager::GetInstance().GetMouseX()+Camera::pos.x, InputManager::GetInstance().GetMouseY()+Camera::pos.y};

    // Update timer and box position
    cooldown.Update(dt);
    associated.box.Centered(pbody.lock()->box.Center());

    // Update Box rotation degree (always staring mouse direction)
    Vec2 direction = mouseCoord - associated.box.Center();
    associated.angleDeg = direction.SlopeX()*180/PI;

    // will only shoot every .5 seconds
    if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) and cooldown.Get()>=0.5f){
        Shoot();
        cooldown.Restart();
    }
}

void PenguinCannon::Shoot(){
    GameObject* bulletObject = new GameObject();
    // Bullet(associated, angle, speed, damage, maxDistance, sprite, frameTime, frameCount)
    bulletObject->AddComponent(new Bullet(*bulletObject, associated.angleDeg*PI/180, 500, 20, 500, "assets/img/penguinbullet.png", 0.05, 4, false));

    // calculating bullet starting point
    Vec2 offset = Vec2::Rotate(Vec2(associated.box.x+associated.box.w, associated.box.y+associated.box.h/2) - associated.box.Center(), associated.angleDeg*PI/180);
    Vec2 start =  associated.box.Center() + offset;

    bulletObject->box.x = start.x - bulletObject->box.w/2;
    bulletObject->box.y = start.y;
    bulletObject->angleDeg = associated.angleDeg;
    Game::GetInstance().GetCurrentState().AddObject(bulletObject);
}

void PenguinCannon::Render(){}

void PenguinCannon::NotifyCollision(GameObject& other){}

bool PenguinCannon::Is(std::string type){
    if(type.compare("Penguin cannon") == 0){
        return true;
    }

    return false;
}