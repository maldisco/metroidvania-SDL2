#include "StageState.h"
#include "Sound.h"
#include "TileMap.h"
#include "Game.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraFollower.h"
#include "Alien.h"
#include "PenguinBody.h"
#include "Collision.cpp"
#include "TitleState.h"
#include "EndState.h"
#include "GameData.h"

StageState::StageState() : State(), backgroundMusic("assets/audio/stageState.ogg"){    
	// background
	GameObject* bg = new GameObject();
	bg->AddComponent(new Sprite("assets/img/ocean.jpg", *bg, 1, 1.0));
	bg->AddComponent(new CameraFollower(*bg));
	bg->box.x = 0;
	bg->box.y = 0;
	AddObject(bg);

	// tileset
    GameObject* tileMap = new GameObject();
	TileSet* tileSet = new TileSet(*tileMap, 64, 64, "assets/img/tileset.png");
	tileMap->AddComponent(new TileMap(*tileMap, "assets/map/tileMap.txt", tileSet));
	tileMap->box.x = 0;
	tileMap->box.y = 0;
	AddObject(tileMap);

	// main char
	GameObject* penguin = new GameObject();
	penguin->AddComponent(new PenguinBody(*penguin));
	penguin->box.x = 704;
	penguin->box.y = 640;
	AddObject(penguin);

	// make penguin as camera focus
	Camera::Follow(penguin);
    
	// enemy
	GameObject* alien = new GameObject();
	alien->AddComponent(new Alien(*alien));
	alien->box.x = 512 - alien->box.w/2;
	alien->box.y = 300 - alien->box.h/2;
	AddObject(alien);

	// enemy
	alien = new GameObject();
	alien->AddComponent(new Alien(*alien, 0.035f));
	alien->box.x = 1012 - alien->box.w/2;
	alien->box.y = 100 - alien->box.h/2;
	AddObject(alien);

	// enemy
	alien = new GameObject();
	alien->AddComponent(new Alien(*alien, 0.07f));
	alien->box.x = 166 - alien->box.w/2;
	alien->box.y = 500 - alien->box.h/2;
	AddObject(alien);

	// enemy
	alien = new GameObject();
	alien->AddComponent(new Alien(*alien, 0.1f));
	alien->box.x = 20 - alien->box.w/2;
	alien->box.y = 20 - alien->box.h/2;
	AddObject(alien);
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

void StageState::LoadAssets(){}

void StageState::Pause(){
	backgroundMusic.Stop();
}

void StageState::Resume(){
	backgroundMusic.Play();
}

void StageState::Update(float dt){
	// update camera
	Camera::Update(dt);

	// check if quit was requested
	if(InputManager::GetInstance().QuitRequested()){
		quitRequested = true;
	}

	if(InputManager::GetInstance().KeyPress(ESCAPE_KEY)){
        popRequested = true;
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

	if(Alien::alienCount <= 0){
		GameData::playerVictory = true;
		popRequested = true;
		Game::GetInstance().Push(new EndState());
	}

	if(PenguinBody::player == nullptr){
		GameData::playerVictory = false;
		popRequested = true;
		Game::GetInstance().Push(new EndState());
	}
}

void StageState::Render(){
	// Render every object
    RenderArray();
}


