#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_WIDTH 1600
#define CAMERA_HEIGHT 900
#define MAX_SHAKE 30.0f
#define MAX_ANGLE 5

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
     * @brief add some trauma to camera (shake)
     * 
     * @param qt 
     */
    static void AddTrauma(float qt);
    static void SetPos(float x, float y, Rect mapBox);

    /**
     * @brief Squirrel Eiserloh noise function
     * 
     * @param position 
     * @return uint32_t 
     */
    static uint32_t NoiseFun(int position);


    static Vec2 pos, virtualPos, dest;
    static Rect window;
    static Rect panicBox;

    static float angle, virtualAngle;

    // PENSE EM TROCAR O LERP POR CUBIC SPLINE

    static float trauma;
    // things add 0.1-0.2 trauma
    // trauma decreases linearly
    // trauma is applied ² or ³

    // maybe use
    static Rect anchor;

private:
    static GameObject *focus;
    static int noisePos;
};
#endif