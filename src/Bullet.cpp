#include "Bullet.h"
#include "sprite.h"
#include "Collider.h"

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, std::string sprite, float frameTime, int frameCount, bool targetsPlayer) :
 Component(associated), distanceLeft(maxDistance), damage(damage), targetsPlayer(targetsPlayer){
    associated.AddComponent(new Sprite(sprite, associated, frameCount, frameTime));
    associated.AddComponent(new Collider(associated));

    this->speed = Vec2::Rotate(Vec2(speed, 0), angle);
} 

void Bullet::Update(float dt){
    associated.box = associated.box + speed*dt;

    distanceLeft -= Vec2::Magnitude(speed*dt);
    if(distanceLeft <= 0){
        associated.RequestDelete();
    }
}

void Bullet::Render(){}

void Bullet::NotifyCollision(GameObject& other){
    if(targetsPlayer){
        if(other.GetComponent("Penguin body") != nullptr){
            associated.RequestDelete();
        }
    } else {
        if(other.GetComponent("Alien") != nullptr){
            associated.RequestDelete();
        }
    }
}

bool Bullet::Is(std::string type){
    if(type.compare("Bullet") == 0){
        return true;
    }

    return false;
}

int Bullet::GetDamage(){ return damage; }
