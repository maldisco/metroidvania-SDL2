#include "Camera.h"
#include "InputManager.h"

Vec2 Camera::pos = Vec2(0, 0);
Vec2 Camera::speed = Vec2(100, 100);
GameObject* Camera::focus = nullptr;
void Camera::Follow(GameObject* newFocus){
    focus = newFocus;
}

void Camera::Unfollow(){
    focus = nullptr;
}

void Camera::Reset(){
    Camera::pos.x = 0;
    Camera::pos.y = 0;
}

void Camera::Update(float dt){
    if(focus == nullptr){
        if(InputManager::GetInstance().IsKeyDown(LEFT_ARROW_KEY)){
            pos.x -= speed.x*dt;
        } 
        if (InputManager::GetInstance().IsKeyDown(RIGHT_ARROW_KEY)){
            pos.x += speed.x*dt;
        } 
        if (InputManager::GetInstance().IsKeyDown(UP_ARROW_KEY)){
            pos.y -= speed.y*dt;
        } 
        if (InputManager::GetInstance().IsKeyDown(DOWN_ARROW_KEY)){
            pos.y += speed.y*dt;
        }   
    } else {
        pos.x = focus->box.Center().x - CAMERA_WIDTH/2;
        pos.y = focus->box.Center().y - CAMERA_HEIGHT/2;
    }
}