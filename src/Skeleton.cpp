#include "Skeleton.h"
#include "Collider.h"
#include "Game.h"
#include "Damage.h"
#include "StageState.h"
#include "Player.h"
#include "InputManager.h"

Skeleton::Skeleton(GameObject& associated) : Being(associated, {75, 0}, 1.0f, 100), cooldown(){
    associated.AddComponent(new Sprite(SKELETON_IDLE_FILE, associated, 4, 0.05f));
    associated.AddComponent(new Collider(associated, {32/associated.box.w, 64/associated.box.h}));
}

Skeleton::~Skeleton(){

}

void Skeleton::Start(){

}

void Skeleton::Update(float dt){
    // Useful objects
    StageState state = (StageState&)Game::GetInstance().GetCurrentState();
    TileMap* tileMap = state.GetTileMap();
    TileSet* tileSet = state.GetTileSet();
    Sprite* sprite = (Sprite*)associated.GetComponent("Sprite");
    Collider* collider = (Collider*)associated.GetComponent("Collider");
    InputManager& inputManager = InputManager::GetInstance();
    
    // check if in ground
    bool grounded = false;
    int tileLeftX = collider->box.x/tileSet->GetTileWidth();
    int tileRightX = (collider->box.x+collider->box.w)/tileSet->GetTileWidth();
    int tileBottomY = (collider->box.y+collider->box.h+1)/tileSet->GetTileHeight();
    if(tileMap->IsSolid(tileLeftX, tileBottomY) or tileMap->IsSolid(tileRightX, tileBottomY)){
        grounded = true;
    }

    speed.y = speed.y + GRAVITY;
    switch(charState){
        case IDLE:
            // Actions

            // State change conditions
            if(grounded and Rect::Distance(Player::player->GetBox(), associated.box) <= 640){
                sprite->Change(SKELETON_RUN_FILE, 0.05f, 12);
                charState = WALKING;
            }
            break;

        case WALKING:
            // Actions
            // - update timer (will walk for 1 sec)
            cooldown.Update(dt);

            // - update horizontal speed
            moveX(speed.x*dt, collider->box, tileMap, tileSet);   
            
            // - update direction
            if(cooldown.Get() >= 3.0f){
                speed.x = speed.x*-1;
                dir = dir*-1;
                cooldown.Restart();
            } 

            // State change conditions
            if (Rect::Distance(Player::player->GetBox(), associated.box) > 640){
                sprite->Change(SKELETON_IDLE_FILE, 0.05f, 4);
                charState = IDLE;
                cooldown.Restart();
            }
            break;

        case HURT:
            // Actions

            
            // State change conditions
            if(sprite->GetCurrentFrame() == sprite->GetFrameCount()-1){
                sprite->Change(SKELETON_IDLE_FILE, 0.05f, 4);
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

    // - update vertical speed (all states do)
    moveY(speed.y*dt + (GRAVITY*(dt*dt))/2, collider->box, tileMap, tileSet);

    sprite->SetDir(dir);
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
    if(other.GetComponent("Damage") != nullptr){
        Damage* damage = (Damage*)other.GetComponent("Damage");
        if(not damage->targetsPlayer and not (charState == DEAD or charState == HURT)){
            Sprite* sprite = (Sprite*)associated.GetComponent("Sprite");
            this->hp -= damage->GetDamage();
            this->charState = HURT;
            sprite->Change(SKELETON_HURT_FILE, 0.05, 3);
            if(this->hp <= 0){
                this->charState = DEAD;
                sprite->Change(SKELETON_DEATH_FILE, 0.05, 13);
            }
        }
    }
}