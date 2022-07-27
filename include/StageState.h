#ifndef STAGESTATE_H
#define STAGESTATE_H

#include "State.h"
#include "Music.h"
#include "Sprite.h"
#include "TileSet.h"
#include "TileMap.h"

/**
 * @brief Class responsible for controlling the screen and specific internal logics.
 */
class StageState : public State{
    private:
        Music backgroundMusic;
        TileSet* tileSet;     
        TileMap* tileMap;

    public:
        StageState(int stage = 0);
        ~StageState();

        /**
         * @brief Update game objects
         * 
         * @param dt 
         */
        void Update(float dt);

        /**
         * @brief Render game objects
         */
        void Render();
        void LoadAssets();
        void Start();
        void Pause();
        void Resume();

        TileMap* GetTileMap();
        TileSet* GetTileSet();
};
#endif
