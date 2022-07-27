#ifndef PLAYER_H
#define PLAYER_H

#define MAX_SPEEDH 200
#define JUMP_FORCE 300
#define JUMP_HOLD  30

#define PLAYER_JUMP_FILE            "assets/img/warrior-jump.png"
#define PLAYER_RUN_FILE             "assets/img/warrior-run.png"
#define PLAYER_IDLE_FILE            "assets/img/warrior-idle.png"
#define PLAYER_FALL_FILE            "assets/img/warrior-fall.png"
#define PLAYER_ATTACK1_FILE         "assets/img/warrior-attack1.png"
#define PLAYER_ATTACK2_FILE         "assets/img/warrior-attack2.png"
#define PLAYER_CROUCH_FILE          "assets/img/warrior-crouch.png"
#define PLAYER_DASH_FILE            "assets/img/warrior-dash.png"
#define PLAYER_DASHATTACK_FILE      "assets/img/warrior-dash-attack.png"
#define PLAYER_DEATH_FILE           "assets/img/warrior-death.png"
#define PLAYER_SLIDE_FILE           "assets/img/warrior-slide.png"
#define PLAYER_HURT_FILE            "assets/img/warrior-hurt.png"


#include "Being.h"
#include "Timer.h"

class Player : public Being {
    public:
        /**
         * @brief Construct a new Red Hood object
         * 
         * @param associated 
         */
        Player(GameObject& associated);
        ~Player();

        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);

        void Idle();
        void Attack();
        void Attack2();
        void Jump();
        void Walk();
        void Dash(int dir);
        void Fall();

        static Player* player;
    private:
        int combo, jumpCounter;
        bool invincible;
        Timer invincibleTime;
        Timer jumpImpulse;
};
#endif