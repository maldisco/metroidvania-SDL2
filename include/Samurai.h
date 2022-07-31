#ifndef SAMURAI_H
#define SAMURAI_H
#include "Being.h"
#include "Timer.h"

#define SAMURAI_IDLE_FILE "assets/img/sekiro-idle.png"
#define SAMURAI_MOVE_FILE "assets/img/sekiro-run.png"
#define SAMURAI_HURT_FILE "assets/img/sekiro-hurt.png"
#define SAMURAI_DEAD_FILE "assets/img/sekiro-death.png"
#define SAMURAI_DASH_FILE "assets/img/sekiro-dashattack.png"
#define SAMURAI_ATTACK_FILE "assets/img/sekiro-slash.png"

class Samurai : public Being
{
public:
    /**
     * @brief Construct a new Samurai object
     *
     * @param associated
     */
    Samurai(GameObject &associated);
    ~Samurai();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void Start();
    void NotifyCollision(GameObject &other);

private:
    Timer cooldown, dashTime;
    Vec2 dashPosLeft, dashPosRight;
};
#endif