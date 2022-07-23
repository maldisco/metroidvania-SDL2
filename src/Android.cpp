#include "Android.h"
#include "Sprite.h"
#include "Collider.h"
#include "Game.h"
#include "Camera.h"
#include "Bullet.h"
#include "Timer.h"
#include "Sound.h"
#include "InputManager.h"
#include "TileMap.h"
#include "StageState.h"

Android* Android::player;
Android::Android(GameObject& associated) : Component(associated), speed({100,150}), mass(77.38f), hp(100), playerState(STANDING){
    player = this;
    associated.AddComponent(new Sprite("assets/img/2bstand.png", associated, 3, 0.2f));
    associated.AddComponent(new Collider(associated));
}

Android::~Android(){
    player = nullptr;
}

void Android::Start(){
    
}

void Android::Update(float dt){
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
    int left = inputManager.IsKeyDown(A_KEY) ? 1 : 0;
    int right = inputManager.IsKeyDown(D_KEY) ? 1 : 0;
    int jump = inputManager.KeyPress(W_KEY) ? 1 : 0;
    
    std::set<int> xAxis;
    float motionY, closestObstacleY, distance;
    // State machine
    switch(playerState){
        case STANDING:
            // Actions
            // - update vertical speed
            motionY = speed.y*dt + (GRAVITY*(dt*dt))/2;
            xAxis.emplace(tileLeftX);
            xAxis.emplace(tileRightX);
            closestObstacleY = TileMap->ScanY(xAxis, (associated.box.y+associated.box.h)/tileSet->GetTileHeight());
            distance = closestObstacleY*tileSet->GetTileHeight() - (associated.box.y+associated.box.h);
            motionY = std::min(motionY, distance);
            associated.box.y += motionY;

            // State change condition
            if(inputManager.KeyPress(A_KEY) or inputManager.KeyPress(D_KEY)){
                sprite->Change("assets/img/2bwalk.png", 0.1f, 6);
                playerState = WALKING;
            } else if(jump){
                sprite->Change("assets/img/2bjump.png", 0.2f, 3, 1);
                speed.y = (-400000)*dt/mass;
                playerState = JUMPING;
            }
            break;
        
        case WALKING:
            // Actions
            // - update horizontal speed
            speed.x = (right - left) * (MAX_SPEEDH*dt);
            if(speed.x){
                // set of rows player intersects
                std::set<std::pair<int, int>> tiles;
                for(int i = associated.box.y; i < associated.box.y+associated.box.h; i++){
                    tiles.emplace((associated.box.x + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                }

                for(auto tile : tiles){
                    if(TileMap->IsSolid(tile.first, tile.second)){
                        speed.x = 0;
                    }
                }   
                associated.box.x += speed.x;    

                sprite->SetDir(right - left);
            }

            // State change condition
            if (not( inputManager.IsKeyDown(A_KEY) or inputManager.IsKeyDown(D_KEY) ) and ( inputManager.KeyRelease(A_KEY) or inputManager.KeyRelease(D_KEY) )){
                sprite->Change("assets/img/2bstand.png", 0.2f, 3);
                playerState = STANDING;
            } else if(jump){
                sprite->Change("assets/img/2bjump.png", 0.1f, 3, 1);
                speed.y = (-400000)*dt/mass;
                playerState = JUMPING;
            } else if (not grounded){
                sprite->Change("assets/img/2bfall.png", 0.1f, 3, 1);
                playerState = FALLING;
            }
            break;
        
        case JUMPING:
            // Actions
            // - update horizontal speed
            speed.x = (right - left) * (MAX_SPEEDH*dt);
            if(speed.x){
                // set of rows player intersects
                std::set<std::pair<int, int>> tiles;
                for(int i = associated.box.y; i < associated.box.y+associated.box.h; i++){
                    tiles.emplace((associated.box.x + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                }

                for(auto tile : tiles){
                    if(TileMap->IsSolid(tile.first, tile.second)){
                        speed.x = 0;
                    }
                }   
                associated.box.x += speed.x;    

                sprite->SetDir(right - left);
            }

            // - update vertical speed
            motionY = speed.y*dt + (GRAVITY*(dt*dt))/2;
            xAxis.emplace(tileLeftX);
            xAxis.emplace(tileRightX);
            closestObstacleY = TileMap->ScanY(xAxis, (associated.box.y+associated.box.h)/tileSet->GetTileHeight());
            distance = closestObstacleY*tileSet->GetTileHeight() - (associated.box.y+associated.box.h);
            motionY = std::min(motionY, distance);
            associated.box.y += motionY;

            // State change condition
            if(speed.y > 0){
                sprite->Change("assets/img/2bfall.png", 0.1f, 3, 1);
                playerState = FALLING;
            }
            break;
        
        case FALLING:
            // Actions
            // - update horizontal speed
            speed.x = (right - left) * (MAX_SPEEDH*dt);
            if(speed.x){
                // set of rows player intersects
                std::set<std::pair<int, int>> tiles;
                for(int i = associated.box.y; i < associated.box.y+associated.box.h; i++){
                    tiles.emplace((associated.box.x + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                }

                for(auto tile : tiles){
                    if(TileMap->IsSolid(tile.first, tile.second)){
                        speed.x = 0;
                    }
                }   
                associated.box.x += speed.x;    

                sprite->SetDir(right - left);
            }

            // - update vertical speed
            motionY = speed.y*dt + (GRAVITY*(dt*dt))/2;
            xAxis.emplace(tileLeftX);
            xAxis.emplace(tileRightX);
            closestObstacleY = TileMap->ScanY(xAxis, (associated.box.y+associated.box.h)/tileSet->GetTileHeight());
            distance = closestObstacleY*tileSet->GetTileHeight() - (associated.box.y+associated.box.h);
            motionY = std::min(motionY, distance);
            associated.box.y += motionY;

            // State change condition
            if(grounded){
                if(inputManager.IsKeyDown(A_KEY) or inputManager.IsKeyDown(D_KEY)){
                    sprite->Change("assets/img/2bwalk.png", 0.1f, 6);
                    playerState = WALKING;
                } else {
                    sprite->Change("assets/img/2bstand.png", 0.2f, 3);
                    playerState = STANDING;
                }
            }
            break;
    }
}

void Android::NotifyCollision(GameObject& other){
    if(other.GetComponent("Bullet") != nullptr){
        Bullet* bullet = (Bullet*)other.GetComponent("Bullet");
        if(bullet->targetsPlayer){
            this->hp -= 5;
            if(this->hp <= 0){
            }
        }
    }
}

void Android::Render(){}

bool Android::Is(std::string type){
    if(type.compare("Android") == 0){
        return true;
    }

    return false;
}