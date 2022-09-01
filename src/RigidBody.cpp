#include "RigidBody.h"
#include "TileMap.h"
#include "TileSet.h"
#include "StageState.h"
#include "Game.h"

RigidBody::RigidBody(GameObject &associated, float gravityScale) : Component(associated), velocity(), gravityScale(gravityScale), gravity(80)
{
}

void RigidBody::Start()
{
    collider = GetComponent<Collider>();
}

void RigidBody::Update(float dt)
{
    MoveX(dt);
    MoveY(dt);
    collider->Update(dt);
    ApplyGravity();
}

void RigidBody::ApplyGravity()
{
    if (not collider->IsGrounded())
        velocity = Vec2(velocity.x, velocity.y + (gravity * gravityScale));
    else
        velocity = Vec2(velocity.x, 0);
}

void RigidBody::MoveX(float dt)
{
    TileMap *tileMap = static_cast<StageState &>(Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = static_cast<StageState &>(Game::GetInstance().GetCurrentState()).GetTileSet();

    float distance, closestObstacleX;
    std::set<int> yAxis;
    for (int i = collider->box.y; i < collider->box.y + collider->box.h; i++)
    {
        yAxis.emplace(i / tileSet->GetTileHeight());
    }

    float motionX = this->velocity.x * dt;
    if (motionX >= 0)
    {
        closestObstacleX = tileMap->ScanRight(yAxis, (collider->box.x + collider->box.w) / tileSet->GetTileWidth());
        distance = (closestObstacleX * tileSet->GetTileWidth() - 1) - (collider->box.x + collider->box.w);
        motionX = std::min(motionX, distance);
    }
    else
    {
        closestObstacleX = tileMap->ScanLeft(yAxis, collider->box.x / tileSet->GetTileWidth());
        distance = (closestObstacleX * tileSet->GetTileWidth() + tileSet->GetTileWidth()) - collider->box.x;
        motionX = std::max(motionX, distance);
    }

    associated.box.x += motionX;
}

void RigidBody::MoveY(float dt)
{
    TileMap *tileMap = static_cast<StageState &>(Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = static_cast<StageState &>(Game::GetInstance().GetCurrentState()).GetTileSet();

    float distance, closestObstacleY;
    int tileLeftX = collider->box.x / tileSet->GetTileWidth();
    int tileRightX = (collider->box.x + collider->box.w) / tileSet->GetTileWidth();
    std::set<int> xAxis = {tileLeftX, tileRightX}; // set of columns player intersects

    float motion = this->velocity.y * dt;
    if (motion >= 0)
    {
        closestObstacleY = tileMap->ScanDown(xAxis, (collider->box.y + collider->box.h) / tileSet->GetTileHeight());
        distance = (closestObstacleY * tileSet->GetTileHeight() - 1) - (collider->box.y + collider->box.h);
        motion = std::min(motion, distance);
    }
    else
    {
        closestObstacleY = tileMap->ScanUp(xAxis, (collider->box.y) / tileSet->GetTileHeight());
        distance = (closestObstacleY * tileSet->GetTileHeight() + tileSet->GetTileHeight()) - (collider->box.y);
        // distance (and motion) here are negative values, thats why max
        motion = std::max(motion, distance);
    }

    associated.box.y += motion;
}