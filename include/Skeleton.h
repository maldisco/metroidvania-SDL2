#ifndef SKELETON_H
#define SKELETON_H
#include "Being.h"

#define SKELETON_IDLE_FILE          "assets/img/skeletonidle.png"
#define SKELETON_RUN_FILE           "assets/img/skeletonrun.png"
#define SKELETON_DEATH_FILE         "assets/img/skeletondeath.png"
#define SKELETON_ATTACK_FILE        "assets/img/skeletonattack.png"
#define SKELETON_HURT_FILE          "assets/img/skeletonhurt.png"

class Skeleton : public Being {
    public:
        Skeleton(GameObject& associated);
        ~Skeleton();

        void Update(float dt);
        void Render();
        bool Is(std::string type);

        void Start();
        void NotifyCollision(GameObject& other);
};
#endif