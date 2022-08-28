#include "Hud.h"
#include "Sprite.h"
#include "CameraFollower.h"
#include "GameData.h"

Hud::Hud(GameObject &associated, int maxHp) : Component(associated), maxHp(maxHp), hpbar(new Sprite(HPBAR, associated))
{
    // Decrease or increase hp bar
    hpbar->SetClipScale(static_cast<float>(GameData::playerHp) / static_cast<float>(maxHp), hpbar->GetClipScale().y);
    associated.AddComponent(this);
    associated.AddComponent(new Sprite(HPBAR_BG, associated));
    associated.AddComponent(hpbar);
    associated.AddComponent(new CameraFollower(associated, {30, 15}));
}

void Hud::Render()
{
}

void Hud::Update(float dt)
{
    // Decrease or increase hp bar
    hpbar->SetClipScale(static_cast<float>(GameData::playerHp) / static_cast<float>(maxHp), hpbar->GetClipScale().y);
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