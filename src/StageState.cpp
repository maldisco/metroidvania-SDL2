#include "StageState.h"
#include "Sound.h"
#include "TileMap.h"
#include "Game.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraFollower.h"
#include "Player.h"
#include "Skeleton.h"
#include "Samurai.h"
#include "Slime.h"
#include "Collision.cpp"
#include "Collider.h"
#include "TitleState.h"
#include "PauseState.h"
#include "EndState.h"
#include "Resources.h"
#include "GameData.h"
#include "Sensor.h"
#include "Hud.h"
#include "BossHud.h"
#include "Npc.h"
#include "Text.h"

StageState::StageState(int stage) : State(), backgroundMusic("assets/audio/cinnabar.mp3")
{
	// all stages have a map (with the same tileset)
	GameObject *map = new GameObject();
	tileSet = new TileSet(*map, 64, 64, "assets/img/foresTileset.png");

	// all stages have a player
	GameObject *player = new GameObject();
	player->AddComponent(new Player(*player));
	player->box.x = GameData::playerPos.x;
	player->box.y = GameData::playerPos.y;

	// all stages have HUD
	GameObject *hud = new GameObject();
	hud->AddComponent(new Hud(*hud, 5));

	GameObject *enemy = new GameObject();
	GameObject *sensor = new GameObject();
	GameObject *background = new GameObject();
	GameObject *boss = new GameObject();
	GameObject *bosshud = new GameObject();
	GameObject *npc = new GameObject();
	switch (stage)
	{
	case 0:
		background = new GameObject();
		background->AddComponent(new TileMap(*background, "assets/map/background.tmj", tileSet, true));
		background->box.x = 0;
		background->box.y = 0;
		AddObject(background);

		npc = new GameObject();
		npc->AddComponent(new Npc(*npc, "assets/img/bluewitch.png", "assets/dialogues/bluewitch.txt"));
		npc->box.x = 20 * tileSet->GetTileWidth();
		npc->box.y = 875;
		AddObject(npc);

		AddObject(player);

		tileMap = new TileMap(*map, "assets/map/room0.tmj", tileSet);
		map->AddComponent(tileMap);
		map->box.x = 0;
		map->box.y = 0;
		AddObject(map);

		sensor = new GameObject();
		sensor->AddComponent(new Sensor(*sensor, {35 * tileSet->GetTileWidth(), 2 * tileSet->GetTileHeight()}, 1));
		sensor->AddComponent(new Collider(*sensor));
		sensor->box.x = 66 * tileSet->GetTileWidth();
		sensor->box.y = 19 * tileSet->GetTileHeight();
		sensor->box.w = tileSet->GetTileWidth() * 4;
		sensor->box.h = tileSet->GetTileHeight();
		AddObject(sensor);
		break;

	case 1:
		background = new GameObject();
		background->AddComponent(new TileMap(*background, "assets/map/background.tmj", tileSet, true));
		background->box.x = 0;
		background->box.y = 0;
		AddObject(background);

		enemy = new GameObject();
		enemy->AddComponent(new Slime(*enemy));
		enemy->box.x = 500;
		enemy->box.y = 80;
		AddObject(enemy);

		enemy = new GameObject();
		enemy->AddComponent(new Skeleton(*enemy));
		enemy->box.x = 700;
		enemy->box.y = 80;
		AddObject(enemy);

		enemy = new GameObject();
		enemy->AddComponent(new Slime(*enemy));
		enemy->box.x = 100;
		enemy->box.y = 80;
		AddObject(enemy);

		AddObject(player);

		tileMap = new TileMap(*map, "assets/map/room1.tmj", tileSet);
		map->AddComponent(tileMap);
		map->box.x = 0;
		map->box.y = 0;
		AddObject(map);

		sensor = new GameObject();
		sensor->AddComponent(new Sensor(*sensor, {tileSet->GetTileWidth(), 8 * tileSet->GetTileHeight()}, 2));
		sensor->AddComponent(new Collider(*sensor));
		sensor->box.x = 74 * tileSet->GetTileWidth();
		sensor->box.y = 10 * tileSet->GetTileHeight();
		sensor->box.w = tileSet->GetTileWidth();
		sensor->box.h = tileSet->GetTileHeight() * 4;
		AddObject(sensor);

		sensor = new GameObject();
		sensor->AddComponent(new Sensor(*sensor, {64 * tileSet->GetTileWidth(), 15 * tileSet->GetTileHeight()}, 0));
		sensor->AddComponent(new Collider(*sensor));
		sensor->box.x = 35 * tileSet->GetTileWidth();
		sensor->box.y = 1 * tileSet->GetTileHeight();
		sensor->box.w = tileSet->GetTileWidth() * 4;
		sensor->box.h = tileSet->GetTileHeight();
		AddObject(sensor);
		break;

	case 2:
		background = new GameObject();
		TileSet *bossRoomTS = new TileSet(*background, 64, 64, "assets/img/duskTileSet.png");
		background->AddComponent(new TileMap(*background, "assets/map/room2bg.tmj", bossRoomTS, true));
		background->box.x = 0;
		background->box.y = 0;
		AddObject(background);

		if (not GameData::samuraiSlain)
		{
			boss = new GameObject();
			boss->AddComponent(new Samurai(*boss));
			boss->box.x = 17 * tileSet->GetTileWidth();
			boss->box.y = 9 * tileSet->GetTileHeight();
			AddObject(boss);

			bosshud = new GameObject();
			bosshud->AddComponent(new BossHud(*bosshud, boss, 20));
		}

		AddObject(player);

		tileMap = new TileMap(*map, "assets/map/room2.tmj", tileSet);
		map->AddComponent(tileMap);
		map->box.x = 0;
		map->box.y = 0;
		AddObject(map);

		sensor = new GameObject();
		sensor->AddComponent(new Sensor(*sensor, {71 * tileSet->GetTileWidth(), 10 * tileSet->GetTileHeight()}, 1));
		sensor->AddComponent(new Collider(*sensor));
		sensor->box.x = 0 * tileSet->GetTileWidth();
		sensor->box.y = 8 * tileSet->GetTileHeight();
		sensor->box.w = tileSet->GetTileWidth();
		sensor->box.h = tileSet->GetTileHeight() * 4;
		AddObject(sensor);

		if (not GameData::samuraiSlain)
			AddObject(bosshud);
		break;
	}

	// Add hud in front of everything onscreen
	AddObject(hud);

	// make android as camera focus
	Camera::SetPos(player->box.x - 5 * tileSet->GetTileWidth(), player->box.y - 5 * tileSet->GetTileHeight(), tileMap->GetBox());
	Camera::Follow(player);
}

StageState::~StageState()
{
	objectArray.clear();
	backgroundMusic.Stop();
}

void StageState::Start()
{
	LoadAssets();
	StartArray();
	backgroundMusic.Play();
	started = true;
}

void StageState::LoadAssets()
{
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
	std::cout << "Carregando sprites do inimigo numero2...";
	Resources::GetImage(SLIME_IDLE_FILE);
	Resources::GetImage(SLIME_MOVE_FILE);
	Resources::GetImage(SLIME_HURT_FILE);
	Resources::GetImage(SLIME_DEATH_FILE);
	Resources::GetImage(SLIME_ATTACK_FILE);
	std::cout << "Carregando fonte...\n";
	Resources::GetFont(PEABERRY_FONT, 150);
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

	if (Player::player == nullptr)
	{
		if (InputManager::GetInstance().KeyPress(SPACE_KEY))
		{
			popRequested = true;
		}
	}

	// check if pause was requested
	if (InputManager::GetInstance().KeyPress(ESCAPE_KEY))
	{
		Game::GetInstance().Push(new PauseState());
	}

	// Update every object
	UpdateArray(dt);

	// check collidable objects
	std::vector<std::weak_ptr<GameObject>> collidable = QueryObjectsBy("Collider");
	for (unsigned i = 0; i < collidable.size(); i++)
	{
		for (unsigned j = i + 1; j < collidable.size(); j++)
		{
			Collider *collider1 = (Collider *)collidable[i].lock()->GetComponent("Collider");
			Collider *collider2 = (Collider *)collidable[j].lock()->GetComponent("Collider");
			if (Collision::IsColliding(collider1->box, collider2->box, collidable[i].lock()->angleDeg * PI / 180, collidable[j].lock()->angleDeg * PI / 180))
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
