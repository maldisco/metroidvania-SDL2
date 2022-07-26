#include "Being.h"

Being::Being(GameObject& associated, Vec2 speed, float mass, int hp, int dir) : Component(associated), speed(speed), mass(mass), hp(hp), charState(IDLE), dir(dir){

}

Being::~Being(){

}

void Being::moveX(float motion, Rect colliderBox, TileMap* tileMap, TileSet* tileSet){
    std::set<std::pair<int, int>> tiles;
    for(int i = colliderBox.y; i < colliderBox.y+colliderBox.h; i++){
        if(motion >= 0) tiles.emplace((colliderBox.x + colliderBox.w + motion)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
        else tiles.emplace((colliderBox.x + motion)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
    }

    for(auto tile : tiles){
        if(tileMap->IsSolid(tile.first, tile.second)){
            motion = 0;
        }
    }

    associated.box.x += motion;   
}

void Being::moveY(float motion, Rect colliderBox, TileMap* tileMap, TileSet* tileSet){
    float distance, closestObstacleY;
    int tileLeftX = colliderBox.x/tileSet->GetTileWidth();
    int tileRightX = (colliderBox.x+colliderBox.w)/tileSet->GetTileWidth();
    std::set<int> xAxis = {tileLeftX, tileRightX}; // set of columns player intersects
    if(motion >= 0) {
        closestObstacleY = tileMap->ScanDown(xAxis, (colliderBox.y+colliderBox.h)/tileSet->GetTileHeight());
        distance = closestObstacleY*tileSet->GetTileHeight() - (colliderBox.y+colliderBox.h);
        motion = std::min(motion, distance);
    } else {
        closestObstacleY = tileMap->ScanUp(xAxis, (colliderBox.y)/tileSet->GetTileHeight());
        distance = closestObstacleY*tileSet->GetTileHeight() - (colliderBox.y);
        // distance (and motion) here are negative values, thats why max
        motion = std::max(motion, distance);
    }
    associated.box.y += motion;
}

int Being::GetDir(){ return dir; }

Vec2 Being::GetSpeed(){ return speed; }

bool Being::Grounded() { return grounded; }