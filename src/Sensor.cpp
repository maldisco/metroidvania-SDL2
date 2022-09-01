#include "Sensor.h"
#include "Player.h"
#include "StageState.h"
#include "Transition.h"

Sensor::Sensor(GameObject &associated, Vec2 destinyPlayerPos, int destiny) : Component(associated), destiny(destiny), destinyPlayerPos(destinyPlayerPos)
{
}

void Sensor::Update(float dt)
{
}

void Sensor::NotifyCollision(GameObject &other)
{
    if (other.GetComponent<Player>() != nullptr)
    {
        GameData::playerPos.x = destinyPlayerPos.x;
        GameData::playerPos.y = destinyPlayerPos.y;
        Transition::Instance->StartTransition(new StageState(destiny));
    }
}