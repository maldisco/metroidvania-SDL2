#include "StageState.h"
#include "Sound.h"
#include "TileMap.h"
#include "Game.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraFollower.h"
#include "RedHood.h"
#include "Skeleton.h"
#include "Collision.cpp"
#include "TitleState.h"
#include "EndState.h"
#include "Resources.h"
#include "GameData.h"

StageState::StageState() : State(), backgroundMusic("assets/audio/cinnabar.mp3"){    
	// background
	GameObject* bg = new GameObject();
	bg->AddComponent(new Sprite("assets/img/background.png", *bg, 1, 1.0));
	bg->box.x = 0;
	bg->box.y = 0;
	AddObject(bg);

	// tileset
    GameObject* map = new GameObject();
	tileSet = new TileSet(*map, 32, 32, "assets/img/tileset.png");
	tileMap = new TileMap(*map, "assets/map/map.txt", tileSet);
	map->AddComponent(tileMap);
	map->box.x = 0;
	map->box.y = 0;
	AddObject(map);

	// main char
	GameObject* player = new GameObject();
	player->AddComponent(new RedHood(*player));
	player->box.x = 300;
	player->box.y = 50;
	AddObject(player);

	// enemy 1
	GameObject* enemy = new GameObject();
	enemy->AddComponent(new Skeleton(*enemy));
	enemy->box.x = 500;
	enemy->box.y = 50;
	AddObject(enemy);

	// make android as camera focus
	Camera::Follow(player);
}

StageState::~StageState(){
	objectArray.clear();
}

void StageState::Start(){
	LoadAssets();
	StartArray();
	backgroundMusic.Play();
	started = true;
}

void StageState::LoadAssets(){
	std::cout << "Carregando sprites do personagem principal..." << "\n";
	Resources::GetImage(PLAYER_IDLE_FILE);
	Resources::GetImage(PLAYER_RUN_FILE);
	Resources::GetImage(PLAYER_JUMP_FILE);
	Resources::GetImage(PLAYER_FALL_FILE);
	Resources::GetImage(PLAYER_LAND_FILE);
	Resources::GetImage(PLAYER_LIGHTATTACK1_FILE);
	Resources::GetImage(PLAYER_LIGHTATTACK2_FILE);
	Resources::GetImage(PLAYER_LIGHTATTACK3_FILE);
	std::cout << "Carregando sprites do inimigo numero 1..." << "\n";
	Resources::GetImage(SKELETON_IDLE_FILE);
	Resources::GetImage(SKELETON_RUN_FILE);
	Resources::GetImage(SKELETON_HURT_FILE);
	Resources::GetImage(SKELETON_DEATH_FILE);
	Resources::GetImage(SKELETON_ATTACK_FILE);
}

void StageState::Pause(){
	backgroundMusic.Stop();
}

void StageState::Resume(){
	backgroundMusic.Play();
}

void StageState::Update(float dt){
	// check if quit was requested
	if(InputManager::GetInstance().QuitRequested()){
		quitRequested = true;
	}

	if(InputManager::GetInstance().KeyPress(ESCAPE_KEY)){
        popRequested = true;
		backgroundMusic.Stop();
    }

	// Update every object
    UpdateArray(dt);

	// check collidable objects
	std::vector<std::weak_ptr<GameObject>> collidable = QueryObjectsBy("Collider");
	for(unsigned i=0; i<collidable.size(); i++){
		for(unsigned j=i+1; j<collidable.size(); j++){
			if(Collision::IsColliding(collidable[i].lock()->box, collidable[j].lock()->box, collidable[i].lock()->angleDeg*PI/180, collidable[j].lock()->angleDeg*PI/180)){
				GameObject* g1 = collidable[i].lock().get();
				GameObject* g2 = collidable[j].lock().get();
				g1->NotifyCollision(*g2);
				g2->NotifyCollision(*g1);
			}
		}
	}

	// Check if object is dead
    for(unsigned i=0; i<objectArray.size(); i++){
        if( objectArray[i]->IsDead() ){
            objectArray.erase(objectArray.begin()+i);
        }
    }

	// update camera
	Camera::Update(dt);
}

void StageState::Render(){
	// Render every object
    RenderArray();
}

TileMap* StageState::GetTileMap(){ return tileMap; }
TileSet* StageState::GetTileSet(){ return tileSet; }

