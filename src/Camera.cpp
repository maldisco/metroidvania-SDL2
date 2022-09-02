#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "Collider.h"
#include "Game.h"
#include "StageState.h"
#include "Helpers.h"

Vec2 Camera::pos = Vec2(448, 2469);
Vec2 Camera::virtualPos = Vec2(448, 2469);

Rect Camera::window = Rect(576, 350, 384, 250);
Rect Camera::panicBox = Rect(0, 50, 0, 600);

float Camera::trauma = 0.0f;
float Camera::lookAhead = 100;

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
        Rect mapBox = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileMap()->GetBox();

        Vec2 distance = GetNextPos() - pos;
        pos.x = Helpers::Clamp(0, pos.x + distance.x * 0.1f, mapBox.w - CAMERA_WIDTH);
        if(not (window+pos).Contains(focus->box.Center()))
            pos.y = Helpers::Clamp(0, pos.y + distance.y * 0.1f, mapBox.h - CAMERA_HEIGHT);        
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

void Camera::AddTrauma(float qt)
{
    trauma += qt;
}

void Camera::SetPos(float x, float y, Rect mapBox)
{
    pos.x = Helpers::Clamp(0, x, mapBox.w - CAMERA_WIDTH);
    pos.y = Helpers::Clamp(0, y, mapBox.h - CAMERA_HEIGHT);
}

Vec2 Camera::GetNextPos()
{
    return Vec2(focus->box.Center().x - CAMERA_WIDTH/2 + Helpers::Sign(focus->direction)*lookAhead, focus->box.Center().y - CAMERA_HEIGHT/2);
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