#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"
#include "Collider.h"

class RigidBody : public Component
{
public:
    Vec2 velocity;
    float gravityScale;

    RigidBody(GameObject& associated, float gravityScale = 1);
    void Start();
    void Update(float dt);

private:
    Collider *collider;
    int tileHeight, tileWidth;
    float gravity;

    void MoveX(float dt);
    void MoveY(float dt);   
    void ApplyGravity(); 
};

#endif