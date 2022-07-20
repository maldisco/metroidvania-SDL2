#include "CameraFollower.h"
#include "Camera.h"

CameraFollower::CameraFollower(GameObject& associated) : Component(associated){}

void CameraFollower::Update(float dt){
    associated.box.x = Camera::pos.x;
    associated.box.y = Camera::pos.y;
}

void CameraFollower::Render(){}
bool CameraFollower::Is(std::string type){
    if(type.compare("Camera follower") == 0){
        return true;
    }

    return false;
} 
void CameraFollower::NotifyCollision(GameObject& other){}