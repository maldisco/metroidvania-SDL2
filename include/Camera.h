#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_WIDTH 1600
#define CAMERA_HEIGHT 900
#define SHAKE 30.0f

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
    static void AddTrauma(float qt);
    static void SetPos(float x, float y, Rect mapBox);


    static Vec2 pos, virtualPos, dest;
    static Rect window;
    static Rect panicBox;

    // PENSE EM TROCAR O LERP POR CUBLIC SPLINE

    static float trauma;
    // things add 0.1-0.2 trauma
    // trauma decreases linearly
    // trauma is applied ² or ³

    // maybe use
    static Rect anchor;

private:
    static GameObject *focus;
};
#endif