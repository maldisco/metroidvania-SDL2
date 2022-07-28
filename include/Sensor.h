#ifndef SENSOR_H
#define SENSOR_H
#include "Component.h"

class Sensor : public Component
{
public:
    Sensor(GameObject &associated, int destiny = 0);
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject &other);

private:
    int destiny;
};
#endif