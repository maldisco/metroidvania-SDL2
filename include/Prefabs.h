#ifndef PREFABS_H
#define PREFABS_H

#include "GameObject.h"
#include "Player.h"
#include "Sprite.h"
#include "Transform.h"
#include "Health.h"
#include "Animator.h"
#include "Gravitypp.h"
#include "RigidBody.h"
#include "Skeleton.h"
#include "Hud.h"
#include "Transition.h"
#include "DialogueManager.h"

class Prefabs
{
public:

    /**
     * @brief Generates game object of a player
     * 
     * @param x position X
     * @param y position Y
     * @return GameObject* 
     */
    static GameObject *PlayerPrefab(float x = 0, float y = 0)
    {
        GameObject *playerPrefab = new GameObject(x, y, Enums::Player);
        Sprite *sprite = new Sprite(PLAYER_IDLE_FILE, *playerPrefab, 6, 0.05f);
        playerPrefab->AddComponent(new Collider(*playerPrefab, {60 / playerPrefab->box.w, 128 / playerPrefab->box.h}, {0, 20}));
        playerPrefab->AddComponent(new Transform(*playerPrefab, Vec2(75, 0)));
        playerPrefab->AddComponent(new Player(*playerPrefab));
        playerPrefab->AddComponent(new Health(*playerPrefab, 10));
        playerPrefab->AddComponent(new Animator(*playerPrefab, sprite));
        playerPrefab->AddComponent(sprite);
        playerPrefab->AddComponent(new Gravitypp(*playerPrefab, 1.5f));
        playerPrefab->AddComponent(new RigidBody(*playerPrefab));

        return playerPrefab;
    }

    /**
     * @brief Generate a game object of a skeleton
     * 
     * @param x position X
     * @param y position Y
     * @return GameObject* 
     */
    static GameObject *SkeletonPrefab(float x = 0, float y = 0)
    {
        GameObject *skeletonPrefab = new GameObject(x, y, Enums::Enemy);
        skeletonPrefab->AddComponent(new Sprite(SKELETON_IDLE_FILE, *skeletonPrefab, 4, 0.05f));
        skeletonPrefab->AddComponent(new Collider(*skeletonPrefab, {64 / skeletonPrefab->box.w, 128 / skeletonPrefab->box.h}));
        skeletonPrefab->AddComponent(new RigidBody(*skeletonPrefab));
        skeletonPrefab->AddComponent(new Health(*skeletonPrefab, 3));
        skeletonPrefab->AddComponent(new Skeleton(*skeletonPrefab));

        return skeletonPrefab;
    }
    
    /**
     * @brief Generates a game object of a sensor
     * 
     * @param x Position X
     * @param y Position Y
     * @param w Width
     * @param h Height
     * @param playerX Coords sensor takes player to
     * @param playerY Coords sensor takes player to
     * @param destiny Stage sensor takes player to
     * @return GameObject* 
     */
    static GameObject *SensorPrefab(float x, float y, float w, float h, float playerX, float playerY, int destiny)
    {
        GameObject *sensorPrefab = new GameObject(x, y);
        sensorPrefab->AddComponent(new Sensor(*sensorPrefab, {playerX, playerY}, destiny));
		sensorPrefab->AddComponent(new Collider(*sensorPrefab));
		sensorPrefab->box.w = w;
		sensorPrefab->box.h = h;

        return sensorPrefab;
    }

    /**
     * @brief Generates the game object of npc Magda
     * 
     * @param x Position x
     * @param y Position Y
     * @return GameObject* 
     */
    static GameObject *MagdaPrefab(float x, float y)
    {
        GameObject *npc = new GameObject(x, y);
        npc->AddComponent(new Sprite("assets/img/bluewitch.png", *npc, 6, 0.1f));
        npc->AddComponent(new Collider(*npc));
		npc->AddComponent(new Npc(*npc, "assets/dialogues/bluewitch.txt"));

        return npc;
    }

    /**
     * @brief Generates the game object of HpBar (HUD)
     * 
     * @param x Position X
     * @param y Position Y
     * @return GameObject* 
     */
    static GameObject *HpBarPrefab(float x, float y)
    {
        GameObject *bar = new GameObject();
        Sprite *hpbar = new Sprite(HPBAR, *bar);
        bar->AddComponent(new Hud(*bar, hpbar));
        bar->AddComponent(new Sprite(HPBAR_BG, *bar));
        bar->AddComponent(hpbar);
        bar->AddComponent(new CameraFollower(*bar, {x, y}));

        return bar;
    }

    /**
     * @brief Generates the game object of a transition
     * 
     * @param duration transition duration
     * @return GameObject* 
     */
    static GameObject *TransitionPrefab(float duration)
    {
        GameObject *transition = new GameObject();
        transition->AddComponent(new Transition(*transition, duration));

        return transition;
    }

    /**
     * @brief Generates the game object of a tilemap
     * 
     * @param tileSet 
     * @param tileWidth 
     * @param tileHeight 
     * @param tileMap 
     * @param background bool
     * @return GameObject* 
     */
    static GameObject *TileMapPrefab(std::string tileSet, int tileWidth, int tileHeight, std::string tileMap, bool background = false)
    {
        GameObject *map = new GameObject();
        map->AddComponent(new TileMap(*map, tileMap, new TileSet(*map, tileWidth, tileHeight, tileSet), background));

        return map;
    }

    /**
     * @brief Generates the game object of a dialogue box
     * 
     * @return GameObject* 
     */
    static GameObject *DialogueBoxPrefab()
    {
        GameObject *box = new GameObject();
        Sprite *square = new Sprite("assets/img/square.png", *box);
        square->SetScale(1, 1/3);
        square->SetTransparency(0.8);
        box->AddComponent(square);
        box->AddComponent(new CameraFollower(*box, Vec2(400, 100)));

        return box;
    }

    /**
     * @brief Generates the game object of a dialogue text
     * 
     * @return GameObject* 
     */
    static GameObject *DialogueTextPrefab()
    {
        GameObject *dialText = new GameObject();
        dialText->AddComponent(new Text(*dialText, PEABERRY_FONT, 30, Text::BLENDED, "Test", {255, 255, 255, SDL_ALPHA_OPAQUE}));
        dialText->AddComponent(new CameraFollower(*dialText, Vec2(450, 120)));

        return dialText;
    }

    /**
     * @brief Generates the game object of a dialogue name
     * 
     * @return GameObject* 
     */
    static GameObject *DialogueNamePrefab()
    {
        GameObject *dialName = new GameObject();
        dialName->AddComponent(new Text(*dialName, PEABERRY_FONT, 60, Text::BLENDED, "Name", {255, 255, 255, SDL_ALPHA_OPAQUE}));
        dialName->AddComponent(new CameraFollower(*dialName, Vec2(50, 700)));

        return dialName;
    }

    /**
     * @brief Generates the game object of a dilogue manager
     * 
     * @param box Dialogue box game object
     * @param text Dialogue text game object
     * @param name Dialogue name game object
     * @return GameObject* 
     */
    static GameObject *DialogueManagerPrefab(GameObject *box, GameObject *text, GameObject *name)
    {
        GameObject *dialManager = new GameObject();
        dialManager->AddComponent(new DialogueManager(*dialManager, *box, *text, *name));

        return dialManager;
    }
};

#endif