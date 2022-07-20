#include "StageState.h"
#include "Sound.h"
#include "TileMap.h"
#include "Game.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraFollower.h"
#include "Android.h"
#include "Collision.cpp"
#include "TitleState.h"
#include "EndState.h"
#include "GameData.h"

StageState::StageState() : State(), backgroundMusic("assets/audio/cinnabar.mp3"){    
	// background
	GameObject* bg = new GameObject();
	bg->AddComponent(new Sprite("assets/img/background.png", *bg, 1, 1.0));
	//bg->AddComponent(new CameraFollower(*bg));
	bg->box.x = 0;
	bg->box.y = 0;
	AddObject(bg);

	// tileset
    GameObject* tileMap = new GameObject();
	TileSet* tileSet = new TileSet(*tileMap, 32, 32, "assets/img/tileset.png");
	tileMap->AddComponent(new TileMap(*tileMap, "assets/map/map.txt", tileSet));
	tileMap->box.x = 0;
	tileMap->box.y = 0;
	AddObject(tileMap);

	// main char
	GameObject* android = new GameObject();
	android->AddComponent(new Android(*android));
	android->box.x = 20;
	android->box.y = 50;
	AddObject(android);

	// make android as camera focus
	Camera::Follow(android);
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
}

void StageState::Render(){
	// Render every object
    RenderArray();
}


