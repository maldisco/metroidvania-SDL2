#include "Skeleton.h"
#include "Collider.h"
#include "Game.h"
#include "StageState.h"
#include "InputManager.h"

Skeleton::Skeleton(GameObject& associated) : Being(associated, {100, 0}, 50.0f, 100){
    associated.AddComponent(new Sprite(SKELETON_IDLE_FILE, associated, 4, 0.05f));
    associated.AddComponent(new Collider(associated));
}

Skeleton::~Skeleton(){

}

void Skeleton::Start(){

}

void Skeleton::Update(float dt){
    // Useful objects
    StageState state = (StageState&)Game::GetInstance().GetCurrentState();
    TileMap* TileMap = state.GetTileMap();
    TileSet* tileSet = state.GetTileSet();
    Sprite* sprite = (Sprite*)associated.GetComponent("Sprite");
    InputManager& inputManager = InputManager::GetInstance();
    
    // check if in ground
    bool grounded = false;
    int tileLeftX = associated.box.x/tileSet->GetTileWidth();
    int tileRightX = (associated.box.x+associated.box.w)/tileSet->GetTileWidth();
    int tileBottomY = (associated.box.y+associated.box.h+1)/tileSet->GetTileHeight();
    if(TileMap->IsSolid(tileLeftX, tileBottomY) or TileMap->IsSolid(tileRightX, tileBottomY)){
        grounded = true;
    }

    speed.y = speed.y + GRAVITY;
    int hit =  inputManager.KeyPress(SPACE_KEY);

    switch(charState){
        case IDLE:
            // Actions


            // State change conditions
            if(hit){
                this->hp -= 20;
                if(hp <= 0){
                    sprite->Change(SKELETON_DEATH_FILE, 0.05, 13);
                    charState = DEAD;
                } else {
                    sprite->Change(SKELETON_HURT_FILE, 0.05, 3);
                    charState = HURT;
                }

            }
            break;
        case HURT:
            // Actions

            
            // State change conditions
            if(sprite->GetCurrentFrame() == sprite->GetFrameCount()-1){
                sprite->Change(SKELETON_IDLE_FILE, 0.05, 4);
                charState = IDLE;
            }
            break;
        
        case DEAD:
            // Actions

            // State change conditions
            if(sprite->GetCurrentFrame() == sprite->GetFrameCount()-1){
                associated.RequestDelete();
            }
            break;
    }

    std::set<int> xAxis;
    // - update vertical speed
    float motionY = speed.y*dt + (GRAVITY*(dt*dt))/2;
    xAxis.emplace(tileLeftX);
    xAxis.emplace(tileRightX);
    float closestObstacleY = TileMap->ScanY(xAxis, (associated.box.y+associated.box.h)/tileSet->GetTileHeight());
    float distance = closestObstacleY*tileSet->GetTileHeight() - (associated.box.y+associated.box.h);
    motionY = std::min(motionY, distance);
    associated.box.y += motionY;
}

void Skeleton::Render(){

}

bool Skeleton::Is(std::string type){
    if(type.compare("Skeleton") == 0){
        return true;
    }

    return false;
}

void Skeleton::NotifyCollision(GameObject& other){
    if(other.GetComponent("RedHood") != nullptr){
         
    }
}