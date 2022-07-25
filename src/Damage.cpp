#include "Damage.h"
#include "sprite.h"
#include "Collider.h"

Damage::Damage(GameObject& associated, int damage, bool targetsPlayer, float msToSelfDestruct) : Component(associated), damage(damage), targetsPlayer(targetsPlayer),
 lifeTime(), msToSelfDestruct(msToSelfDestruct){
    associated.AddComponent(new Collider(associated));
} 

void Damage::Update(float dt){
    lifeTime.Update(dt);

    if(lifeTime.Get() >= msToSelfDestruct){
        associated.RequestDelete();
    }
}

void Damage::Render(){}

void Damage::NotifyCollision(GameObject& other){
    if(targetsPlayer){
        if(other.GetComponent("Player") != nullptr){
            associated.RequestDelete();
        }
    } else {
        if(other.GetComponent("Skeleton") != nullptr){
            associated.RequestDelete();
        }
    }
}

bool Damage::Is(std::string type){
    if(type.compare("Damage") == 0){
        return true;
    }

    return false;
}

int Damage::GetDamage(){ return damage; }
