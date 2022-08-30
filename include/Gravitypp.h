#ifndef GRAVITYPP_H
#define GRAVITYPP_H
#include "Component.h"
#include "RigidBody.h"
#include "InputManager.h"

class Gravitypp : public Component
{
public:
    Gravitypp(GameObject &associated, float fallMultiplier);
    void Start();
    void Update(float dt);

private:
    float fallMultiplier;
    RigidBody *rigidBody;
};
#endif