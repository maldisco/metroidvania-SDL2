#include "RedHood.h"
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

RedHood* RedHood::player;
RedHood::RedHood(GameObject& associated) : Being(associated, {100, 150}, 77.38f, 100), combo(0){
    player = this;
    associated.AddComponent(new Sprite(PLAYER_IDLE_FILE, associated, 18, 0.05f));
    associated.AddComponent(new Collider(associated, {1, 1}, {0, 28}));
}

RedHood::~RedHood(){
    player = nullptr;
}

void RedHood::Start(){
    
}

void RedHood::Update(float dt){
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
    int lightAttack = inputManager.KeyPress(Z_KEY) ? 1 : 0;
    
    std::set<int> xAxis;
    float motionY, closestObstacleY, distance;
    // State machine
    switch(charState){
        case IDLE:
            // Actions

            // State change condition
            if(inputManager.KeyPress(A_KEY) or inputManager.KeyPress(D_KEY)){
                sprite->Change(PLAYER_RUN_FILE, 0.05f, 24);
                charState = WALKING;
            } else if(jump){
                sprite->Change(PLAYER_JUMP_FILE, 0.2f, 10, 3);
                speed.y = (-JUMP_FORCE)*dt/mass;
                charState = JUMPING;
            } else if(not grounded){
                sprite->Change(PLAYER_FALL_FILE, 0.05f, 5, 1);
                charState = FALLING;
            } else if(lightAttack){
                sprite->Change(PLAYER_LIGHTATTACK1_FILE, 0.05f, 7);
                charState = ATTACKING; 
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
                sprite->Change(PLAYER_IDLE_FILE, 0.05f, 18);
                charState = IDLE;
            } else if(jump){
                sprite->Change(PLAYER_JUMP_FILE, 0.05f, 10, 3);
                speed.y = (-JUMP_FORCE)*dt/mass;
                charState = JUMPING;
            } else if (not grounded){
                sprite->Change(PLAYER_FALL_FILE, 0.05f, 5, 1);
                charState = FALLING;
            } else if (lightAttack){
                sprite->Change(PLAYER_LIGHTATTACK1_FILE, 0.05f, 7);
                charState = ATTACKING; 
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

            // State change condition
            if(speed.y > 0){
                sprite->Change(PLAYER_FALL_FILE, 0.05f, 5, 1);
                charState = FALLING;
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

            // State change condition
            if(grounded){
                if(inputManager.IsKeyDown(A_KEY) or inputManager.IsKeyDown(D_KEY)){
                    sprite->Change(PLAYER_RUN_FILE, 0.05f, 24);
                    charState = WALKING;
                } else {
                    sprite->Change(PLAYER_IDLE_FILE, 0.05f, 18);
                    charState = IDLE;
                }
            }
            break;

        case ATTACKING:
            // Actions
            if(lightAttack and sprite->GetCurrentFrame() >= sprite->GetFrameCount()-3){
                combo++;
                if(combo == 1){
                    sprite->Change(PLAYER_LIGHTATTACK2_FILE, 0.05f, 7);
                    sprite->SetFrame(0);
                } else if (combo == 2){
                    sprite->Change(PLAYER_LIGHTATTACK3_FILE, 0.05f, 14);
                    sprite->SetFrame(0);
                } 
            }

            // Stage change condition
            if(sprite->GetCurrentFrame() >= sprite->GetFrameCount()-1){
                sprite->Change(PLAYER_IDLE_FILE, 0.05f, 18);
                charState = IDLE;
                combo = 0;
            }
            break;
    }

    // - update vertical speed (all states do)
    motionY = speed.y*dt + (GRAVITY*(dt*dt))/2;
    xAxis.emplace(tileLeftX);
    xAxis.emplace(tileRightX);
    closestObstacleY = TileMap->ScanY(xAxis, (associated.box.y+associated.box.h)/tileSet->GetTileHeight());
    distance = closestObstacleY*tileSet->GetTileHeight() - (associated.box.y+associated.box.h);
    motionY = std::min(motionY, distance);
    associated.box.y += motionY;
}

void RedHood::NotifyCollision(GameObject& other){
    
}

void RedHood::Render(){}

bool RedHood::Is(std::string type){
    if(type.compare("RedHood") == 0){
        return true;
    }

    return false;
}