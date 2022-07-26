#include "StageState.h"
#include "Sound.h"
#include "TileMap.h"
#include "Game.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraFollower.h"
#include "Player.h"
#include "Skeleton.h"
#include "Collision.cpp"
#include "Collider.h"
#include "TitleState.h"
#include "EndState.h"
#include "Resources.h"
#include "GameData.h"

StageState::StageState() : State(), backgroundMusic("assets/audio/cinnabar.mp3"){    
    GameObject* map = new GameObject();
	tileSet = new TileSet(*map, 32, 32, "assets/img/foresTileset.png");
	tileMap = new TileMap(*map, "assets/map/room0.txt", tileSet);
	map->AddComponent(tileMap);
	map->box.x = 0;
	map->box.y = 0;
	AddObject(map);

	// enemy 1
	GameObject* enemy = new GameObject();
	enemy->AddComponent(new Skeleton(*enemy));
	enemy->box.x = 500;
	enemy->box.y = 50;
	AddObject(enemy);

	// main char
	GameObject* player = new GameObject();
	player->AddComponent(new Player(*player));
	player->box.x = 300;
	player->box.y = 50;
	AddObject(player);

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
	std::cout << "Carregando sprites do personagem principal...\n";
	Resources::GetImage(PLAYER_IDLE_FILE);
	Resources::GetImage(PLAYER_RUN_FILE);
	Resources::GetImage(PLAYER_JUMP_FILE);
	Resources::GetImage(PLAYER_FALL_FILE);
	Resources::GetImage(PLAYER_HURT_FILE);
	Resources::GetImage(PLAYER_DEATH_FILE);
	Resources::GetImage(PLAYER_DASH_FILE);
	Resources::GetImage(PLAYER_DASHATTACK_FILE);
	Resources::GetImage(PLAYER_CROUCH_FILE);
	Resources::GetImage(PLAYER_SLIDE_FILE);
	Resources::GetImage(PLAYER_ATTACK1_FILE);
	Resources::GetImage(PLAYER_ATTACK2_FILE);
	std::cout << "Carregando sprites do inimigo numero 1...\n";
	Resources::GetImage(SKELETON_IDLE_FILE);
	Resources::GetImage(SKELETON_RUN_FILE);
	Resources::GetImage(SKELETON_HURT_FILE);
	Resources::GetImage(SKELETON_DEATH_FILE);
	Resources::GetImage(SKELETON_ATTACK_FILE);
	std::cout << "Carregando fonte...\n";
	Resources::GetFont("assets/font/PeaberryBase.ttf", 150);
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

	if(Player::player == nullptr){
		if(InputManager::GetInstance().KeyPress(SPACE_KEY)){
			popRequested = true;
			backgroundMusic.Stop();
		}
	}

	// Update every object
    UpdateArray(dt);

	// check collidable objects
	std::vector<std::weak_ptr<GameObject>> collidable = QueryObjectsBy("Collider");
	for(unsigned i=0; i<collidable.size(); i++){
		for(unsigned j=i+1; j<collidable.size(); j++){
			Collider* collider1 = (Collider*)collidable[i].lock()->GetComponent("Collider");
			Collider* collider2 = (Collider*)collidable[j].lock()->GetComponent("Collider");
			if(Collision::IsColliding(collider1->box, collider2->box, collidable[i].lock()->angleDeg*PI/180, collidable[j].lock()->angleDeg*PI/180)){
				collidable[i].lock()->NotifyCollision(*collidable[j].lock());
				collidable[j].lock()->NotifyCollision(*collidable[i].lock());
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

	// render camera window
	Camera::Render();
}

TileMap* StageState::GetTileMap(){ return tileMap; }
TileSet* StageState::GetTileSet(){ return tileSet; }

