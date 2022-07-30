#include "Sensor.h"
#include "Game.h"
#include "StageState.h"
#include "Camera.h"

Sensor::Sensor(GameObject &associated, Vec2 destinyPlayerPos, int destiny) : Component(associated), destiny(destiny), destinyPlayerPos(destinyPlayerPos)
{
}

void Sensor::Update(float dt)
{
}

void Sensor::Render()
{
}

bool Sensor::Is(std::string type)
{
    if (type.compare("Sensor") == 0)
    {
        return true;
    }

    return false;
}

void Sensor::NotifyCollision(GameObject &other)
{
    if (other.GetComponent("Player") != nullptr)
    {
        Game::GetInstance().GetCurrentState().PopRequested();
        GameData::playerPos.x = destinyPlayerPos.x;
        GameData::playerPos.y = destinyPlayerPos.y;
        Game::GetInstance().Push(new StageState(destiny));
    }
}