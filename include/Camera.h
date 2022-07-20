#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_WIDTH    1024
#define CAMERA_HEIGHT   600

#include "Vec2.h"
#include "GameObject.h"

/**
 * @brief Control screen movement
 * 
 */
class Camera{
    public:
        static void Follow(GameObject* NewFocus);
        static void Unfollow();
        static void Reset();
        static void Update(float dt);
        static Vec2 pos;
        static Vec2 speed;
    
    private:
        static GameObject* focus;
};
#endif