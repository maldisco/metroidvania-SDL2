#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "Collider.h"
#include "Game.h"

Vec2 Camera::pos = Vec2(0, 0);
Vec2 Camera::speed = Vec2(100, 100);
Rect Camera::cameraWindow = Rect(576, 400, 64, 150);
Rect Camera::panic = Rect(0, 50, 0, 600);
float Camera::shakeDur = -1;
float Camera::shakeForce = 5.0f;
Timer Camera::shakeTimer;
int Camera::shakeDir = -1;
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

        if(shakeDur <= 0){
            if(Player::player->GetDir() >= 0) {
                cameraWindow.x = 480;
                if(collider->box.x+collider->box.w > (pos.x+cameraWindow.x+cameraWindow.w)){
                    float dist = ((collider->box.x+collider->box.w) - (pos.x+cameraWindow.x+cameraWindow.w));
                    pos.x = std::min(pos.x + dist/16, 3200.0f);
                }
            } else {
                cameraWindow.x = 672;
                if (collider->box.x < (pos.x+cameraWindow.x)){
                    float dist = (collider->box.x - (pos.x+cameraWindow.x));
                    pos.x = std::max(pos.x + dist/16, 0.0f);
                } 
            }

            if(Player::player->Grounded()){
                float dist = (collider->box.y - (pos.y+cameraWindow.y));
                if(pos.y + dist/8 > 0 and (pos.y + dist/8 + CAMERA_HEIGHT) < 720) pos.y += dist/8;
            } else if ((Player::player->GetBox().y < panic.y) or (Player::player->GetBox().y + Player::player->GetBox().h > panic.y + panic.h)){
                float dist = (collider->box.y - (pos.y+cameraWindow.y));
                if(pos.y + dist/8 > 0 and (pos.y + dist/8 + CAMERA_HEIGHT) < 720) pos.y += dist/8;
            }
        } else {
            shakeTimer.Update(dt);
            if(shakeTimer.Get() < shakeDur){
                pos.x += shakeForce*shakeDir;
                shakeDir = shakeDir*-1;
            } else {
                shakeDur = -1;
                shakeTimer.Restart();
            }
        }
    }
}

void Camera::Render(){
#ifdef DEBUG
    Rect box = cameraWindow+pos;
	Vec2 center( box.Center() );
	SDL_Point points[5];

	Vec2 point = Vec2::Rotate(Vec2(box.x, box.y) - center, 0)
					+ center - Camera::pos;
	points[0] = {(int)point.x, (int)point.y};
	points[4] = {(int)point.x, (int)point.y};
	
	point = Vec2::Rotate(Vec2(box.x+box.w, box.y) - center, 0)
					+ center - Camera::pos;
	points[1] = {(int)point.x, (int)point.y};
	
	point = Vec2::Rotate(Vec2(box.x+box.w, box.y+box.h) - center, 0)
					+ center - Camera::pos;
	points[2] = {(int)point.x, (int)point.y};
	
	point = Vec2::Rotate(Vec2(box.x, box.y+box.h) - center, 0)
					+ center - Camera::pos;
	points[3] = {(int)point.x, (int)point.y};

	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif
}

void Camera::TriggerShake(float time, float force){
    shakeTimer.Restart();
    shakeDur = time;
    shakeForce = force;
    shakeDir = -1;
}