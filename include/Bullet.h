#ifndef BULLET_H
#define BULLET_H

#include "Component.h"

class Bullet : public Component
{
public:
    int damage;

    Bullet(GameObject &associated, float maxDistance, int damage);

    void NotifyCollision(GameObject &other);

private:
    float maxDistance, curDistance;
};

#endif