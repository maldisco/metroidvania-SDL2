#include "Bullet.h"

Bullet::Bullet(GameObject &associated, float maxDistance, int damage) : Component(associated), maxDistance(maxDistance), damage(damage) {}

void Bullet::NotifyCollision(GameObject &other)
{
    if(other.layer == Enums::Player)
    {
        associated.RequestDelete();
    }
}