#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "Collider.h"

Vec2 Camera::pos = Vec2(0, 0);
Vec2 Camera::speed = Vec2(100, 100);
Rect Camera::cameraWindow = Rect(576, 0, 64, 720);
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
    if(focus) {
        Collider* collider = (Collider*)focus->GetComponent("Collider");

        if(Player::player->GetDir() >= 0) {
            cameraWindow.x = 608;
            if(collider->box.x+collider->box.w > (pos.x+cameraWindow.x+cameraWindow.w)){
                float dist = ((collider->box.x+collider->box.w) - (pos.x+cameraWindow.x+cameraWindow.w));
                pos.x += dist/16;
            }
        } else {
            cameraWindow.x = 576;
            if (collider->box.x < (pos.x+cameraWindow.x)){
                float dist = (collider->box.x - (pos.x+cameraWindow.x));
                pos.x += dist/16;
            } 
        }

        int newY = focus->box.Center().y - CAMERA_HEIGHT/2;        
        if(newY > 0 and newY+CAMERA_HEIGHT < 768){
            if(newY >= pos.y){
                pos.y += std::min(CAMERA_SPEED, newY-pos.y);
            } else {
                pos.y += std::max(-CAMERA_SPEED, newY-pos.y);
            }
        }
    }
}