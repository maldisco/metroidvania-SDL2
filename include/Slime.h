#ifndef Slime_H
#define Slime_H
#include "Being.h"
#include "Timer.h"

#define SLIME_IDLE_FILE "assets/img/slimeidle.png"
#define SLIME_MOVE_FILE "assets/img/slimemove.png"
#define SLIME_DEATH_FILE "assets/img/slimedeath.png"
#define SLIME_ATTACK_FILE "assets/img/slimeattack.png"
#define SLIME_HURT_FILE "assets/img/slimehurt.png"

class Slime : public Being
{
public:
    /**
     * @brief Construct a new Slime object
     *
     * @param associated
     */
    Slime(GameObject &associated);
    ~Slime();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void Start();
    void NotifyCollision(GameObject &other);

private:
    Timer cooldown;
};
#endif