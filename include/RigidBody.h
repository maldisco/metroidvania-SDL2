#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"
#include "Collider.h"

class RigidBody : public Component
{
public:
    Vec2 velocity;

    RigidBody(GameObject& associated);
    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);

private:
    Collider *collider;
    int tileHeight, tileWidth;

    void MoveX(float dt);
    void MoveY(float dt);   
    void ApplyGravity(); 
};

#endif