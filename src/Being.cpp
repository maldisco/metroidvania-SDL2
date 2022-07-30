#include "Being.h"

Being::Being(GameObject &associated, Vec2 speed, float mass, int hp, int dir) : Component(associated), speed(speed), mass(mass), hp(hp), knockback(false),
                                                                                knockbackTime(), charState(IDLE), dir(dir)
{
}

Being::~Being()
{
}

void Being::moveX(float motionX, Rect colliderBox, TileMap *tileMap, TileSet *tileSet)
{
    float distance, closestObstacleX;
    std::set<int> yAxis;
    for (int i = colliderBox.y; i < colliderBox.y + colliderBox.h; i++)
    {
        yAxis.emplace(i / tileSet->GetTileHeight());
    }

    if (motionX >= 0)
    {
        closestObstacleX = tileMap->ScanRight(yAxis, (colliderBox.x + colliderBox.w) / tileSet->GetTileWidth());
        distance = (closestObstacleX * tileSet->GetTileWidth() - 1) - (colliderBox.x + colliderBox.w);
        motionX = std::min(motionX, distance);
    }
    else
    {
        closestObstacleX = tileMap->ScanLeft(yAxis, colliderBox.x / tileSet->GetTileWidth());
        distance = (closestObstacleX * tileSet->GetTileWidth() + tileSet->GetTileWidth()) - colliderBox.x;
        motionX = std::max(motionX, distance);
    }

    associated.box.x += motionX;
}

void Being::moveY(float motion, Rect colliderBox, TileMap *tileMap, TileSet *tileSet)
{
    float distance, closestObstacleY;
    int tileLeftX = colliderBox.x / tileSet->GetTileWidth();
    int tileRightX = (colliderBox.x + colliderBox.w) / tileSet->GetTileWidth();
    std::set<int> xAxis = {tileLeftX, tileRightX}; // set of columns player intersects
    if (motion >= 0)
    {
        closestObstacleY = tileMap->ScanDown(xAxis, (colliderBox.y + colliderBox.h) / tileSet->GetTileHeight());
        distance = (closestObstacleY * tileSet->GetTileHeight() - 1) - (colliderBox.y + colliderBox.h);
        motion = std::min(motion, distance);
    }
    else
    {
        closestObstacleY = tileMap->ScanUp(xAxis, (colliderBox.y) / tileSet->GetTileHeight());
        distance = (closestObstacleY * tileSet->GetTileHeight() + tileSet->GetTileHeight()) - (colliderBox.y);
        // distance (and motion) here are negative values, thats why max
        motion = std::max(motion, distance);
    }

    associated.box.y += motion;
}

int Being::GetDir() { return dir; }

int Being::GetHp() { return hp; }

Vec2 Being::GetSpeed() { return speed; }

bool Being::Grounded() { return grounded; }