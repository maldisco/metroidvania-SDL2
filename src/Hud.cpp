#include "Hud.h"
#include "Sprite.h"
#include "CameraFollower.h"
#include "GameData.h"

Hud::Hud(GameObject &associated, int maxHp) : Component(associated), maxHp(maxHp), hpbar(new Sprite(HPBAR, associated))
{
    hpbar->SetRenderWidth((float)GameData::playerHp/(float)maxHp);
    associated.AddComponent(new Sprite(HPBAR_BG, associated));
    associated.AddComponent(hpbar);
    associated.AddComponent(new Sprite(HPBAR_BORDER, associated));
    associated.AddComponent(new CameraFollower(associated, {10, 5}));
}

void Hud::Render()
{
}

void Hud::Update(float dt)
{
    hpbar->SetRenderWidth((float)GameData::playerHp/(float)maxHp);
}

bool Hud::Is(std::string type)
{
    if (type.compare("Hud") == 0)
    {
        return true;
    }

    return false;
}
void Hud::NotifyCollision(GameObject &other) {}