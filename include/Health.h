#ifndef HEALTH_H
#define HEALTH_H

#include "Component.h"

class Health : public Component
{
public:
    Health(GameObject &associated, int maxHp);

    void Start();

    void Damage(int amount);
    void Heal(int amount);
    void HealFull();
    void Kill();
    
    int hp, maxHp;
};

#endif