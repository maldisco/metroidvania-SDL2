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
Android::Android(GameObject& associated) : Component(associated), speed({100,0}), linearSpeed(0), angle(0), hp(100){
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
    InputManager& inputManager = InputManager::GetInstance();

    Vec2 movement = speed*dt;
    Rect newBox = associated.box;

    if (inputManager.IsKeyDown(A_KEY)){
        newBox.x -= movement.x;

        std::set<std::pair<int, int>> tiles;
        for(int i = newBox.y; i < newBox.y+newBox.h; i++){
            tiles.emplace((newBox.x)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
        }

        for(auto tile : tiles){
            if(TileMap->IsSolid(tile.first, tile.second)){
                movement.x = 0;
            }
        }   
        associated.box.x -= movement.x;
    } 
    if (inputManager.IsKeyDown(D_KEY)){
        newBox.x += movement.x;

        std::set<std::pair<int, int>> tiles;
        for(int i = newBox.y; i < newBox.y+newBox.h; i++){
            tiles.emplace((newBox.x+newBox.w)/tileSet->GetTileWidth(), i/tileSet->GetTileHeight());
        }

        for(auto tile : tiles){
            if(TileMap->IsSolid(tile.first, tile.second)){
                movement.x = 0;
            }
        }   
        associated.box.x += movement.x;
    }

    if(inputManager.IsKeyDown(W_KEY)){
        speed.y = -150;
    } 
    // Gravity
    speed.y += GRAVITY;
    newBox = associated.box;
    newBox.y += speed.y*dt;
    std::set<std::pair<int, int>> tiles;
    for(int i = newBox.x; i < newBox.x+newBox.w; i++){
        tiles.emplace(i/tileSet->GetTileWidth(), (newBox.y+newBox.h)/tileSet->GetTileHeight());       
    }
    for(auto tile : tiles){
        if(TileMap->IsSolid(tile.first, tile.second)){
            speed.y = 0;
        }
    }   
    associated.box.y += speed.y*dt;   
}

void Android::NotifyCollision(GameObject& other){
    if(other.GetComponent("Bullet") != nullptr){
        Bullet* bullet = (Bullet*)other.GetComponent("Bullet");
        if(bullet->targetsPlayer){
            this->hp -= 5;
            if(this->hp <= 0){
                Camera::Unfollow();
                associated.RequestDelete();
                pcannon.lock()->RequestDelete();

                GameObject* penguimDeath = new GameObject();
	            penguimDeath->AddComponent(new Sprite("assets/img/penguindeath.png", *penguimDeath, 5, 0.1f, 0.5f));
                Sound* sound = new Sound("assets/audio/boom.wav", *penguimDeath);
                sound->Play();
                penguimDeath->AddComponent(sound);
	            penguimDeath->box.Centered(associated.box.Center());
	            Game::GetInstance().GetCurrentState().AddObject(penguimDeath);
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