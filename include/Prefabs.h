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
};

#endif