#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "Vec2.h"
#include "Rect.h"
#include "Camera.h"

class GameData
{
public:
    static bool playerVictory;
    static bool samuraiSlain;
    static int playerHp;
    static Vec2 playerPos;
    static Vec2 cameraPos;
    static int bluewitchLine;
};
#endif