#include "GameData.h"

bool GameData::playerVictory;
bool GameData::samuraiSlain = false;
int GameData::playerHp = 5;
Vec2 GameData::playerPos = {320, 768};
Rect GameData::dialogueBox = Rect(CAMERA_WIDTH / 2 - 400, 100, 800, 200);