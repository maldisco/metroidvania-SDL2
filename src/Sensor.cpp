#include "Sensor.h"
#include "Game.h"
#include "StageState.h"
#include "Camera.h"
#include "Player.h"

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
        Game::GetInstance().GetCurrentState().PopRequested();
        GameData::playerPos.x = destinyPlayerPos.x;
        GameData::playerPos.y = destinyPlayerPos.y;
        Game::GetInstance().Push(new StageState(destiny));
    }
}