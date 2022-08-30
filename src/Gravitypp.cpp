#include "Gravitypp.h"
#include "RigidBody.h"

Gravitypp::Gravitypp(GameObject& associated, float fallMultiplier) : Component(associated), fallMultiplier(fallMultiplier){}

void Gravitypp::Start()
{
    this->rigidBody = static_cast<RigidBody *>(associated.GetComponent<RigidBody>());
}

void Gravitypp::Update(float dt)
{
    rigidBody->gravityScale = (rigidBody->velocity.y > 0) ? fallMultiplier : 1;
}