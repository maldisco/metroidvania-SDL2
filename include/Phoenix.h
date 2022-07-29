#ifndef PHOENIX_H
#define PHOENIX_H
#include "Being.h"
#include "Timer.h"

#define PHOENIX_IDLE_FILE "assets/img/phoenixidle.png"
#define PHOENIX_MOVE_FILE "assets/img/phoenixmove.png"
#define PHOENIX_HURT_FILE "assets/img/phoenixhurt.png"
#define PHOENIX_DEAD_FILE "assets/img/phoenixdeath.png"
#define PHOENIX_DASH_FILE "assets/img/phoenixdash.png"
#define PHOENIX_ATTACK_FILE "assets/img/phoenixattack.png"

class Phoenix : public Being
{
public:
    /**
     * @brief Construct a new Phoenix object
     *
     * @param associated
     */
    Phoenix(GameObject &associated);
    ~Phoenix();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void Start();
    void NotifyCollision(GameObject &other);

private:
    Timer cooldown, hitCooldown, dashCooldown, dashTime;
};
#endif