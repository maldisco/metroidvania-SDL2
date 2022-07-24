#ifndef PENGUINBODY_H
#define PENGUINBODY_H

#define MAX_SPEEDH 200
#define JUMP_FORCE 600000

#define PLAYER_JUMP_FILE            "assets/img/playerjump.png"
#define PLAYER_RUN_FILE             "assets/img/playerrun.png"
#define PLAYER_IDLE_FILE            "assets/img/playeridle.png"
#define PLAYER_FALL_FILE            "assets/img/playerfall.png"
#define PLAYER_LAND_FILE            "assets/img/playerland.png"
#define PLAYER_LIGHTATTACK1_FILE    "assets/img/playerlightattack1.png"
#define PLAYER_LIGHTATTACK2_FILE    "assets/img/playerlightattack2.png"
#define PLAYER_LIGHTATTACK3_FILE    "assets/img/playerlightattack3.png"


#include "Being.h"

class RedHood : public Being {
    public:
        /**
         * @brief Construct a new Penguin Body object
         * 
         * @param associated 
         */
        RedHood(GameObject& associated);
        ~RedHood();

        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);

        static RedHood* player;
    private:
        int combo;
};
#endif