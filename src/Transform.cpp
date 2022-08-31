#include "Transform.h"
#include "Helpers.h"

Transform::Transform(GameObject &associated, Vec2 offset) : Component(associated), position(associated.box.Center() + offset), offset(offset) {}

void Transform::Update(float dt)
{
    position = associated.box.Center() + offset * Helpers::Sign(associated.direction);
}