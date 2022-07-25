#include "Player.h"
#include "Sprite.h"
#include "Collider.h"
#include "Game.h"
#include "Camera.h"
#include "Damage.h"
#include "Timer.h"
#include "Sound.h"
#include "InputManager.h"
#include "TileMap.h"
#include "StageState.h"

Player* Player::player;
Player::Player(GameObject& associated) : Being(associated, {100, 150}, 1, 100), combo(0){
    player = this;
    associated.AddComponent(new Sprite(PLAYER_IDLE_FILE, associated, 6, 0.05f));
    associated.AddComponent(new Collider(associated, {32/associated.box.w, 64/associated.box.h}, {0, 12}));
}

Player::~Player(){
    player = nullptr;
}

void Player::Start(){
    
}

void Player::Update(float dt){
    // Useful objects
    StageState state = (StageState&)Game::GetInstance().GetCurrentState();
    TileMap* TileMap = state.GetTileMap();
    TileSet* tileSet = state.GetTileSet();
    Sprite* sprite = (Sprite*)associated.GetComponent("Sprite");
    Collider* collider = (Collider*)associated.GetComponent("Collider");
    InputManager& inputManager = InputManager::GetInstance();

    // check if in ground
    bool grounded = false;
    int tileLeftX = collider->box.x/tileSet->GetTileWidth();
    int tileRightX = (collider->box.x+collider->box.w)/tileSet->GetTileWidth();
    int tileBottomY = (collider->box.y+collider->box.h+1)/tileSet->GetTileHeight();
    if(TileMap->IsSolid(tileLeftX, tileBottomY) or TileMap->IsSolid(tileRightX, tileBottomY)){
        grounded = true;
    }

    if(not grounded) speed.y = speed.y + GRAVITY;
    int left = inputManager.IsKeyDown(A_KEY) ? 1 : 0;
    int right = inputManager.IsKeyDown(D_KEY) ? 1 : 0;
    int jump = inputManager.KeyPress(W_KEY) ? 1 : 0;
    int attack = inputManager.KeyPress(Z_KEY) ? 1 : 0;
    int dash = inputManager.KeyPress(SPACE_KEY) ? 1 : 0;
    
    std::set<int> xAxis;
    float motionY, closestObstacleY, distance;
    // State machine
    switch(charState){
        case IDLE:
            // Actions

            // State change condition
            if(inputManager.KeyPress(A_KEY) or inputManager.KeyPress(D_KEY)){
                sprite->Change(PLAYER_RUN_FILE, 0.05f, 8);
                charState = WALKING;
            } else if(jump){
                sprite->Change(PLAYER_JUMP_FILE, 0.05f, 3);
                speed.y = -JUMP_FORCE/mass;
                charState = JUMPING;
            } else if(not grounded){
                sprite->Change(PLAYER_FALL_FILE, 0.05f, 5, 2);
                charState = FALLING;
            } else if(attack){
                sprite->Change(PLAYER_ATTACK1_FILE, 0.05f, 8);
                charState = ATTACKING; 
            } else if (dash){
                sprite->Change(PLAYER_DASH_FILE, 0.05f, 7);
                if(dir >= 0) speed.x = MAX_SPEEDH*2*dt;
                else speed.x = -MAX_SPEEDH*2*dt;
                charState = DASHING;
            }
            break;
    
        case WALKING:
            // Actions
            // - update horizontal speed
            speed.x = (right - left) * (MAX_SPEEDH*dt);
            if(speed.x){
                this->dir = right - left;

                // set of rows player intersects
                std::set<std::pair<int, int>> tiles;
                for(int i = collider->box.y; i < collider->box.y+collider->box.h; i++){
                    if(this->dir >= 0) tiles.emplace((collider->box.x + collider->box.w + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                    else tiles.emplace((collider->box.x + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                }

                for(auto tile : tiles){
                    if(TileMap->IsSolid(tile.first, tile.second)){
                        speed.x = 0;
                    }
                }   
                associated.box.x += speed.x;    
            }

            // State change condition
            if (not( inputManager.IsKeyDown(A_KEY) or inputManager.IsKeyDown(D_KEY) ) and ( inputManager.KeyRelease(A_KEY) or inputManager.KeyRelease(D_KEY) )){
                sprite->Change(PLAYER_IDLE_FILE, 0.05f, 6);
                charState = IDLE;
            } else if(jump){
                sprite->Change(PLAYER_JUMP_FILE, 0.05f, 3);
                speed.y = -JUMP_FORCE/mass;
                charState = JUMPING;
            } else if (not grounded){
                sprite->Change(PLAYER_FALL_FILE, 0.05f, 5, 2);
                charState = FALLING;
            } else if (attack){
                sprite->Change(PLAYER_ATTACK1_FILE, 0.05f, 8);
                charState = ATTACKING; 
            } else if (dash){
                sprite->Change(PLAYER_DASH_FILE, 0.05f, 7);
                if(dir >= 0) speed.x = MAX_SPEEDH*2*dt;
                else speed.x = -MAX_SPEEDH*2*dt;
                charState = DASHING;
            }
            break;
        
        case JUMPING:
            // Actions
            // - update horizontal speed
            speed.x = (right - left) * (MAX_SPEEDH*dt);
            if(speed.x){
                this->dir = right - left;

                // set of rows player intersects
                std::set<std::pair<int, int>> tiles;
                for(int i = collider->box.y; i < collider->box.y+collider->box.h; i++){
                    if(this->dir >= 0) tiles.emplace((collider->box.x + collider->box.w + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                    else tiles.emplace((collider->box.x + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                }

                for(auto tile : tiles){
                    if(TileMap->IsSolid(tile.first, tile.second)){
                        speed.x = 0;
                    }
                }   
                associated.box.x += speed.x;    
                
            }

            // State change condition
            if(speed.y > 0){
                sprite->Change(PLAYER_FALL_FILE, 0.05f, 5, 2);
                charState = FALLING;
            } else if (dash){
                sprite->Change(PLAYER_DASH_FILE, 0.05f, 7);
                if(dir >= 0) speed.x = MAX_SPEEDH*2*dt;
                else speed.x = -MAX_SPEEDH*2*dt;
                charState = DASHING;
            }
            break;
        
        case FALLING:
            // Actions
            // - update horizontal speed
            speed.x = (right - left) * (MAX_SPEEDH*dt);
            if(speed.x){
                this->dir = right-left;

                // set of rows player intersects
                std::set<std::pair<int, int>> tiles;
                for(int i = collider->box.y; i < collider->box.y+collider->box.h; i++){
                    if(this->dir >= 0) tiles.emplace((collider->box.x + collider->box.w + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                    else tiles.emplace((collider->box.x + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                }

                for(auto tile : tiles){
                    if(TileMap->IsSolid(tile.first, tile.second)){
                        speed.x = 0;
                    }
                }   
                associated.box.x += speed.x;  
            }

            // State change condition
            if(grounded){
                if(inputManager.IsKeyDown(A_KEY) or inputManager.IsKeyDown(D_KEY)){
                    sprite->Change(PLAYER_RUN_FILE, 0.05f, 8);
                    charState = WALKING;
                } else {
                    sprite->Change(PLAYER_IDLE_FILE, 0.05f, 6);
                    charState = IDLE;
                }
            } else if (dash){
                sprite->Change(PLAYER_DASH_FILE, 0.05f, 7);
                if(dir >= 0) speed.x = MAX_SPEEDH*2*dt;
                else speed.x = -MAX_SPEEDH*2*dt;
                charState = DASHING;
            }
            break;

        case ATTACKING:
            // Actions
            if(sprite->GetCurrentFrame() == sprite->GetFrameCount()-2){
                GameObject* damage = new GameObject();
                damage->AddComponent(new Damage(*damage, 10*(combo+1), false));
                damage->box.w = 48;
                damage->box.h = 64;
                damage->box.y = collider->box.y;
                if(dir >= 0) damage->box.x = collider->box.x + collider->box.w;
                else damage->box.x = collider->box.x - damage->box.w;

                Game::GetInstance().GetCurrentState().AddObject(damage);
            }

            if(attack and sprite->GetCurrentFrame() >= sprite->GetFrameCount()-2){
                combo++;
                if(combo == 1){
                    sprite->Change(PLAYER_ATTACK2_FILE, 0.05f, 4);
                    sprite->SetFrame(0);
                } 
            }

            // Stage change condition
            if(sprite->GetCurrentFrame() >= sprite->GetFrameCount()-1){
                sprite->Change(PLAYER_IDLE_FILE, 0.05f, 6);
                charState = IDLE;
                combo = 0;
            }
            break;
        
        case DASHING:
            // Actions
            // - update horizontal speed
            if(speed.x){
                // set of rows player intersects
                std::set<std::pair<int, int>> tiles;
                for(int i = collider->box.y; i < collider->box.y+collider->box.h; i++){
                    if(this->dir >= 0) tiles.emplace((collider->box.x + collider->box.w + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                    else tiles.emplace((collider->box.x + speed.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
                }

                for(auto tile : tiles){
                    if(TileMap->IsSolid(tile.first, tile.second)){
                        speed.x = 0;
                    }
                }   
                associated.box.x += speed.x;   
            }

            // State change condition
            if(sprite->GetCurrentFrame() >= sprite->GetFrameCount()-1){
                if(inputManager.IsKeyDown(A_KEY) or inputManager.IsKeyDown(D_KEY)){
                    sprite->Change(PLAYER_RUN_FILE, 0.05f, 8);
                    charState = WALKING;
                } else if(not grounded){
                    if(speed.y >= 0){
                        sprite->Change(PLAYER_FALL_FILE, 0.05f, 5, 2);
                        charState = FALLING;
                    } else {
                        sprite->Change(PLAYER_JUMP_FILE, 0.05f, 3);
                        charState = JUMPING;
                    }
                } else {
                    sprite->Change(PLAYER_IDLE_FILE, 0.05f, 6);
                    charState = IDLE;
                }
            }
            break;
    }

    // - update vertical speed (all states do)
    motionY = speed.y*dt + (GRAVITY*(dt*dt))/2;
    xAxis.emplace(tileLeftX);
    xAxis.emplace(tileRightX);
    closestObstacleY = TileMap->ScanY(xAxis, (collider->box.y+collider->box.h)/tileSet->GetTileHeight());
    distance = closestObstacleY*tileSet->GetTileHeight() - (collider->box.y+collider->box.h);
    motionY = std::min(motionY, distance);
    associated.box.y += motionY;
    
    // sprite direction
    sprite->SetDir(dir);
}

void Player::NotifyCollision(GameObject& other){
    
}

void Player::Render(){}

bool Player::Is(std::string type){
    if(type.compare("Player") == 0){
        return true;
    }

    return false;
}