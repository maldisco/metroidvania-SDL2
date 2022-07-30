#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "Collider.h"
#include "Game.h"
#include "StageState.h"

Vec2 Camera::pos = Vec2(448, 2469);
Vec2 Camera::dest = Vec2(0, 0);

Rect Camera::window = Rect(576, 300, 384, 250);
Rect Camera::panicBox = Rect(0, 50, 0, 600);

float Camera::shakeDur = -1;
Vec2 Camera::shakeForce;
Timer Camera::shakeTimer;
int Camera::shakeDir = -1;
bool Camera::shake = false;

GameObject *Camera::focus = nullptr;
void Camera::Follow(GameObject *newFocus)
{
    focus = newFocus;
}

void Camera::Unfollow()
{
    focus = nullptr;
}

void Camera::Reset()
{
    Camera::pos.x = 0;
    Camera::pos.y = 0;
}

void Camera::Update(float dt)
{
    if (focus)
    {
        Collider *collider = (Collider *)focus->GetComponent("Collider");
        Rect mapBox = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileMap()->GetBox();

        if (Player::player->GetDir() >= 0)
        {
            window.x = 288;

            // Update horizotal position if player gets out of the window
            if (collider->box.x + collider->box.w > (pos.x + window.x + window.w))
            {
                float dist = ((collider->box.x + collider->box.w) - (pos.x + window.x + window.w));
                pos.x = std::min(pos.x + dist / 4, mapBox.w - CAMERA_WIDTH);
            }
        }
        else
        {
            window.x = 736;

            if (collider->box.x < (pos.x + window.x))
            {
                float dist = (collider->box.x - (pos.x + window.x));
                pos.x = std::max(pos.x + dist / 4, 0.0f);
            }
        }

        // Update vertical position when player lands in a platform
        float dist = (collider->box.y - (pos.y + window.y));

        // Only move if new position is inside map
        if (pos.y + dist / 8 > 0 and (pos.y + dist / 8 + CAMERA_HEIGHT) < mapBox.h)
        {
            pos.y += dist / 8;
        }

        if (shake)
        {
            shakeTimer.Update(dt);
            if (shakeTimer.Get() < shakeDur)
            {
                pos = pos + shakeForce * shakeDir;
                shakeDir = shakeDir * -1;
            }
            else
            {
                shakeDur = -1;
                shakeTimer.Restart();
                shake = false;
            }
        }
    }
}

void Camera::Render()
{
#ifdef DEBUG
    Rect box = window + pos;
    Vec2 center(box.Center());
    SDL_Point points[5];

    Vec2 point = Vec2::Rotate(Vec2(box.x, box.y) - center, 0) + center - Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = Vec2::Rotate(Vec2(box.x + box.w, box.y) - center, 0) + center - Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = Vec2::Rotate(Vec2(box.x + box.w, box.y + box.h) - center, 0) + center - Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = Vec2::Rotate(Vec2(box.x, box.y + box.h) - center, 0) + center - Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif
}

void Camera::TriggerShake(float time, Vec2 force)
{
    shakeTimer.Restart();
    shakeDur = time;
    shakeForce = force;
    shakeDir = -1;
    shake = true;
}

void Camera::SetPos(float x, float y, Rect mapBox)
{
    pos.x = std::min(std::max(x, 0.0f), mapBox.w - CAMERA_WIDTH);
    pos.y = std::min(std::max(y, 0.0f), mapBox.h - CAMERA_HEIGHT);
}