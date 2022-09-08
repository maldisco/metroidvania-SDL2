#include "StageState.h"
#include "Sound.h"
#include "TileMap.h"
#include "Game.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraFollower.h"
#include "Player.h"
#include "Skeleton.h"
#include "Collider.h"
#include "PauseState.h"
#include "Resources.h"
#include "GameData.h"
#include "Sensor.h"
#include "Hud.h"
#include "Physics.h"
#include "Npc.h"
#include "Text.h"
#include "Prefabs.h"

StageState::StageState(int stage) : State(), backgroundMusic("assets/audio/cinnabar.mp3"), stage(stage)
{
}

StageState::~StageState()
{
	objectArray.clear();
	backgroundMusic.Stop();
}

void StageState::Start()
{
	LoadStage();
	LoadAssets();
	StartArray();
	backgroundMusic.Play();
	started = true;
}

void StageState::LoadAssets()
{
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
	Resources::GetImage(SKELETON_IDLE_FILE);
	Resources::GetImage(SKELETON_RUN_FILE);
	Resources::GetImage(SKELETON_HURT_FILE);
	Resources::GetImage(SKELETON_DEATH_FILE);
	Resources::GetImage(SKELETON_ATTACK_FILE);
	Resources::GetFont(PEABERRY_FONT, 150);
}

void StageState::LoadStage()
{
	GameObject *map;
	switch (stage)
	{
	case 0:
		map = Prefabs::TileMapPrefab("assets/img/foresTileset.png", 64, 64, "assets/map/room0.tmj");
		tileMap = map->GetComponent<TileMap>();
		tileSet = tileMap->tileSet;
		AddObject(Prefabs::TileMapPrefab("assets/img/foresTileset.png", 64, 64, "assets/map/background.tmj", true));
		AddObject(Prefabs::MagdaPrefab(20 * tileSet->GetTileWidth(), 875));
		AddObject(Prefabs::PlayerPrefab(GameData::playerPos.x, GameData::playerPos.y));
		AddObject(map);
		AddObject(Prefabs::SensorPrefab(66 * tileSet->GetTileWidth(), 19 * tileSet->GetTileHeight(), 4 * tileSet->GetTileWidth(),
										tileSet->GetTileHeight(), 35 * tileSet->GetTileWidth(), 2 * tileSet->GetTileHeight(), 1));
		break;

	case 1:
		map = Prefabs::TileMapPrefab("assets/img/foresTileset.png", 64, 64, "assets/map/room1.tmj");
		tileMap = map->GetComponent<TileMap>();
		tileSet = tileMap->tileSet;
		AddObject(Prefabs::TileMapPrefab("assets/img/foresTileset.png", 64, 64, "assets/map/background.tmj", true));
		AddObject(Prefabs::SkeletonPrefab(700, 80));
		AddObject(Prefabs::SkeletonPrefab(1200, 80));
		AddObject(Prefabs::PlayerPrefab(GameData::playerPos.x, GameData::playerPos.y));
		AddObject(map);
		AddObject(Prefabs::SensorPrefab(74 * tileSet->GetTileWidth(), 10 * tileSet->GetTileHeight(), tileSet->GetTileWidth(),
										4 * tileSet->GetTileHeight(), tileSet->GetTileWidth(), 8 * tileSet->GetTileHeight(), 2));

		AddObject(Prefabs::SensorPrefab(35 * tileSet->GetTileWidth(), tileSet->GetTileHeight(), tileSet->GetTileWidth() * 4,
										tileSet->GetTileHeight(), 64 * tileSet->GetTileWidth(), 15 * tileSet->GetTileHeight(), 0));
		break;

	case 2:
		map = Prefabs::TileMapPrefab("assets/img/foresTileset.png", 64, 64, "assets/map/room2.tmj");
		tileMap = map->GetComponent<TileMap>();
		tileSet = tileMap->tileSet;
		AddObject(Prefabs::TileMapPrefab("assets/img/duskTileSet.png", 64, 64, "assets/map/room2bg.tmj", true));
		AddObject(Prefabs::PlayerPrefab(GameData::playerPos.x, GameData::playerPos.y));
		AddObject(map);
		AddObject(Prefabs::SensorPrefab(0, 8 * tileSet->GetTileHeight(), tileSet->GetTileWidth(), 4 * tileSet->GetTileHeight(),
										71 * tileSet->GetTileWidth(), 10 * tileSet->GetTileHeight(), 1));
		break;
	}
	GameObject *dialogueBox = Prefabs::DialogueBoxPrefab();
	GameObject *dialogueText = Prefabs::DialogueTextPrefab();
	GameObject *dialogueName = Prefabs::DialogueNamePrefab();
	AddObject(Prefabs::DialogueManagerPrefab(dialogueBox, dialogueText, dialogueName));
	AddObject(dialogueBox);
	AddObject(dialogueText);
	AddObject(dialogueName);

	// Add hud in front of everything onscreen
	AddObject(Prefabs::HpBarPrefab(15, 30));
	AddObject(Prefabs::TransitionPrefab(1.5f));

	// make player as camera focus
	Camera::SetPos(Player::player->associated.box.x - 5 * tileSet->GetTileWidth(), Player::player->associated.box.y - 5 * tileSet->GetTileHeight(), tileMap->GetBox());
	Camera::Follow(&Player::player->associated);
}

void StageState::Pause()
{
	backgroundMusic.Stop();
}

void StageState::Resume()
{
	backgroundMusic.Play();
}

void StageState::Update(float dt)
{
	// update camera
	Camera::Update(dt);

	// check if quit was requested
	if (InputManager::GetInstance().QuitRequested())
	{
		quitRequested = true;
	}

	if (Player::player == nullptr and InputManager::GetInstance().KeyPress(SPACE_KEY))
	{
		popRequested = true;
	}

	// check if pause was requested
	if (InputManager::GetInstance().KeyPress(ESCAPE_KEY))
	{
		Game::GetInstance().Push(new PauseState());
	}

	// Update every object
	UpdateArray(dt);

	// check collidable objects
	std::vector<std::weak_ptr<GameObject>> collidable = QueryObjectsBy<Collider>();
	for (unsigned i = 0; i < collidable.size(); i++)
	{
		for (unsigned j = i + 1; j < collidable.size(); j++)
		{
			Collider *collider1 = static_cast<Collider *>(collidable[i].lock()->GetComponent<Collider>());
			Collider *collider2 = static_cast<Collider *>(collidable[j].lock()->GetComponent<Collider>());
			if (Physics::IsColliding(collider1->box, collider2->box, collidable[i].lock()->angleDeg * PI / 180, collidable[j].lock()->angleDeg * PI / 180))
			{
				collidable[i].lock()->NotifyCollision(*collidable[j].lock());
				collidable[j].lock()->NotifyCollision(*collidable[i].lock());
			}
		}
	}

	// Check if object is dead
	for (unsigned i = 0; i < objectArray.size(); i++)
	{
		if (objectArray[i]->IsDead())
		{
			objectArray.erase(objectArray.begin() + i);
		}
	}
}

void StageState::Render()
{
	// Render every object
	RenderArray();

	// render camera window
	Camera::Render();
}

TileMap *StageState::GetTileMap() { return tileMap; }
TileSet *StageState::GetTileSet() { return tileSet; }
