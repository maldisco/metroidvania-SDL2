#ifndef PENGUINBODY_H
#define PENGUINBODY_H

#define MAX_SPEEDH 200
#define JUMP_FORCE 600000

#define PLAYER_JUMP_FILE        "assets/img/playerjump.png"
#define PLAYER_RUN_FILE         "assets/img/playerrun.png"
#define PLAYER_IDLE_FILE        "assets/img/playeridle.png"
#define PLAYER_FALL_FILE        "assets/img/playerfall.png"
#define PLAYER_LAND_FILE        "assets/img/playerland.png"
#define PLAYER_LIGHTATTACK_FILE "assets/img/playerlightattack.png"


#include "Component.h"

class Android : public Component {
    public:
        /**
         * @brief Construct a new Penguin Body object
         * 
         * @param associated 
         */
        Android(GameObject& associated);
        ~Android();

        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);

        static Android* player;
    private:
        Vec2 speed;
        float mass;
        int hp;
        // player state
        enum STATE{ STANDING, WALKING, JUMPING, FALLING, ATTACKING };
        STATE playerState;
};
#endif