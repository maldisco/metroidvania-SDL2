#include "Damage.h"
#include "sprite.h"
#include "Collider.h"
#include "Player.h"
#include "Skeleton.h"

Damage::Damage(GameObject &associated, int damage, bool targetsPlayer, float msToSelfDestruct) : Component(associated), damage(damage), targetsPlayer(targetsPlayer),
                                                                                                 lifeTime(), msToSelfDestruct(msToSelfDestruct)
{
    associated.AddComponent(new Collider(associated));
}

void Damage::Update(float dt)
{
    lifeTime.Update(dt);

    if (lifeTime.Get() >= msToSelfDestruct)
    {
        associated.RequestDelete();
    }
}

void Damage::NotifyCollision(GameObject &other)
{
    if (targetsPlayer)
    {
        if (other.GetComponent<Player>() != nullptr)
        {
            associated.RequestDelete();
        }
    }
    else
    {
        if (other.GetComponent<Skeleton>() != nullptr)
        {
            associated.RequestDelete();
        }
    }
}

int Damage::GetDamage() { return damage; }
