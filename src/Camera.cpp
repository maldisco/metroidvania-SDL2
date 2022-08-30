#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "Collider.h"
#include "Game.h"
#include "StageState.h"

Vec2 Camera::pos = Vec2(448, 2469);
Vec2 Camera::virtualPos = Vec2(448, 2469);

Rect Camera::window = Rect(576, 300, 384, 250);
Rect Camera::panicBox = Rect(0, 50, 0, 600);

float Camera::trauma = 0.0f;

float Camera::angle = 0.0f;
float Camera::virtualAngle = 0.0f;

int Camera::noisePos = 0;

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
    Camera::virtualPos.x = 0;
    Camera::virtualPos.y = 0;
}

void Camera::Update(float dt)
{
    if (focus)
    {
        Collider *collider = (Collider *)focus->GetComponent<Collider>();
        Rect mapBox = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileMap()->GetBox();

        if (Player::player->GetDir() >= 0)
        {
            window.x = 288;

            // Update horizotal position if player gets out of the window
            if (collider->box.x + collider->box.w > (pos.x + window.x + window.w))
            {
                float dist = ((collider->box.x + collider->box.w) - (pos.x + window.x + window.w));
                pos.x = std::min(pos.x + dist * .1f, mapBox.w - CAMERA_WIDTH);
            }
        }
        else
        {
            window.x = 836;

            if (collider->box.x < (pos.x + window.x))
            {
                float dist = (collider->box.x - (pos.x + window.x));
                pos.x = std::max(pos.x + dist * .1f, 0.0f);
            }
        }

        float dist = (collider->box.y - (pos.y + window.y));

        // Only move if new position is inside map
        if (pos.y + dist / 8 > 0 and (pos.y + dist / 8 + CAMERA_HEIGHT) < mapBox.h)
        {
            pos.y += dist / 8;
        }
    }

    // camera shake angle
    float randomShakeAngle = NoiseFun(noisePos++) % 2 == 1 ? static_cast<double>(NoiseFun(noisePos++)) / UINT32_MAX : -static_cast<double>(NoiseFun(noisePos++)) / UINT32_MAX;
    float offsetAngle = MAX_ANGLE * (trauma * trauma) * (randomShakeAngle);
    virtualAngle = angle + offsetAngle;

    // camera shake pos
    float randomShakeX = NoiseFun(noisePos++) % 2 == 1 ? static_cast<double>(NoiseFun(noisePos++)) / UINT32_MAX : -static_cast<double>(NoiseFun(noisePos++)) / UINT32_MAX;
    float randomShakeY = NoiseFun(noisePos++) % 2 == 1 ? static_cast<double>(NoiseFun(noisePos++)) / UINT32_MAX : -static_cast<double>(NoiseFun(noisePos++)) / UINT32_MAX;
    float offsetY = MAX_SHAKE * (trauma * trauma) * randomShakeY;
    float offsetX = MAX_SHAKE * (trauma * trauma) * randomShakeX;
    virtualPos = pos + Vec2(offsetX, offsetY);

    // decrease trauma (shake qt)
    trauma = std::max(0.0f, trauma - 0.05f);
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

void Camera::AddTrauma(float qt)
{
    trauma += qt;
}

void Camera::SetPos(float x, float y, Rect mapBox)
{
    pos.x = std::min(std::max(x, 0.0f), mapBox.w - CAMERA_WIDTH);
    pos.y = std::min(std::max(y, 0.0f), mapBox.h - CAMERA_HEIGHT);
}

uint32_t Camera::NoiseFun(int position)
{
    constexpr unsigned int BIT_NOISE1 = 0xB5297A4D;
    constexpr unsigned int BIT_NOISE2 = 0x68E31DA4;
    constexpr unsigned int BIT_NOISE3 = 0x1B56C4E9;

    unsigned int mangled = position;
    mangled *= BIT_NOISE1;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
}