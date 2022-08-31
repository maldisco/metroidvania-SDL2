#include "Health.h"
#include "Helpers.h"

Health::Health(GameObject &associated, int maxHp) : Component(associated), maxHp(maxHp) {}

void Health::Start()
{
    hp = maxHp;
}

void Health::Heal(int amount)
{
    hp = Helpers::Clamp(0, hp + amount, maxHp);
}

void Health::Damage(int amount)
{
    hp = Helpers::Clamp(0, hp - amount, maxHp);
}

void Health::HealFull()
{
    hp = maxHp;
}

void Health::Kill()
{
    hp = 0;
}