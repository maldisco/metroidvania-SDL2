#ifndef SKELETON_H
#define SKELETON_H

#include "Component.h"
#include "Sprite.h"
#include "Timer.h"
#include "IHittable.h"
#include "RigidBody.h"
#include "Health.h"

#define SKELETON_SPEEDX 300

#define SKELETON_IDLE_FILE "assets/img/skeletonidle.png"
#define SKELETON_RUN_FILE "assets/img/skeletonrun.png"
#define SKELETON_DEATH_FILE "assets/img/skeletondeath.png"
#define SKELETON_ATTACK_FILE "assets/img/skeletonattack.png"
#define SKELETON_HURT_FILE "assets/img/skeletonhurt.png"

class Skeleton : public Component, public IHittable
{
public:
    /**
     * @brief Construct a new Skeleton object
     *
     * @param associated
     */
    Skeleton(GameObject &associated);
    ~Skeleton();

    void Update(float dt);

    void Start();
    void NotifyCollision(GameObject &other);
    void HandleDamage(Rect &box);
    void HandleDeath();

private:
    int hp;
    Timer cooldown;
    Sprite *sprite;
    Collider *collider;
    RigidBody *rigidBody;
    Health *health;

    enum STATE
    {
        IDLE,
        WALKING,
        JUMPING,
        FALLING,
        ATTACKING,
        DASHING,
        HURT,
        DEAD,
        WALLSLIDING
    };
    STATE charState;
};
#endif