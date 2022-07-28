#ifndef COLLIDER_H
#define COLLIDER_H
#include "Component.h"
#include "Rect.h"

#define GRAVITY 50

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
    bool Is(std::string type);
    void NotifyCollision(GameObject &other);

    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);

private:
    Vec2 scale;
    Vec2 offset;
};
#endif