#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_WIDTH 1600
#define CAMERA_HEIGHT 900
#define SHAKE 3.0f

#include "Vec2.h"
#include "GameObject.h"
#include "Timer.h"

/**
 * @brief Control screen movement
 *
 */
class Camera
{
public:
    static void Follow(GameObject *NewFocus);
    static void Unfollow();
    static void Reset();
    static void Update(float dt);
    static void Render();
    /**
     * @brief Trigger a camera shake
     *
     * @param time
     * @param intensity
     */
    static void TriggerShake(float time, Vec2 force);
    static Vec2 pos;
    static Rect window;
    static Rect panicBox;

    static bool shake;
    static Timer shakeTimer;
    static Vec2 shakeForce;
    static float shakeDur;
    static int shakeDir;

    // maybe use
    static Rect anchor;

private:
    static GameObject *focus;
};
#endif