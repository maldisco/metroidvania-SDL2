#include "CameraFollower.h"
#include "Camera.h"
#include "Player.h"

CameraFollower::CameraFollower(GameObject &associated, Vec2 offset) : Component(associated), offset(offset) {}

void CameraFollower::Update(float dt)
{
    associated.box.x = Camera::pos.x + offset.x;
    associated.box.y = Camera::pos.y + offset.y;
}