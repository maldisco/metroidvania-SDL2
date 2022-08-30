#ifndef DOOR_H
#define DOOR_H
#include "Component.h"

class Door : public Component
{
public:
    Door(GameObject &associated, float xAxis, Vec2 matrixPos, int direction = 1);
    void Update(float dt);
    void Render();
    void NotifyCollision(GameObject &other);

private:
    Vec2 matrixPos;
};
#endif