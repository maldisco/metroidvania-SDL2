#ifndef COLLIDER_H
#define COLLIDER_H
#include "Component.h"
#include "Rect.h"

#define GRAVITY 80

// projectile motion
// f(t) = (1/2)gt² + Vo t + Po
// Vo = 2 * h / Th, onde h = altura e Th = tempo para alcançar essa altura
// g = -2 * h / Th²

class Collider : public Component
{
public:
    Rect box;

    /**
     * @brief Construct a new Collider object
     *
     * @param associated
     * @param scale
     * @param offset
     */
    Collider(GameObject &associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});
    void Update(float dt);
    void Render();
    
    bool IsGrounded();

    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);

private:
    Vec2 scale;
    Vec2 offset;
};
#endif