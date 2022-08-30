#ifndef SENSOR_H
#define SENSOR_H
#include "Component.h"
#include "GameData.h"

class Sensor : public Component
{
public:
    Sensor(GameObject &associated, Vec2 destinyPlayerPos, int destiny = 0);
    void Update(float dt);
    void NotifyCollision(GameObject &other);

private:
    int destiny;
    Vec2 destinyPlayerPos;
};
#endif