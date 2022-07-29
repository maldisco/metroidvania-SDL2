#include "BossHud.h"
#include "Sprite.h"
#include "CameraFollower.h"
#include "Camera.h"
#include "Player.h"

BossHud::BossHud(GameObject &associated, GameObject *boss, int maxHp) : Component(associated), boss(boss), maxHp(maxHp), hpbar(new Sprite(BOSS_HPBAR, associated))
{
    associated.AddComponent(new Sprite(BOSS_HPBAR_BG, associated));
    associated.AddComponent(hpbar);
    associated.AddComponent(new Sprite(BOSS_HPBAR_BORDER, associated));
    associated.AddComponent(new CameraFollower(associated, {CAMERA_WIDTH / 2 - 824 / 2, 860}));
}

void BossHud::Render()
{
}

void BossHud::Update(float dt)
{   
    if(((Being *)boss->GetComponent("Samurai"))->GetHp() <= 0)
        associated.RequestDelete();
    
    hpbar->SetRenderWidth((float)((Being *)boss->GetComponent("Samurai"))->GetHp() / (float)maxHp);
}

bool BossHud::Is(std::string type)
{
    if (type.compare("BossHud") == 0)
    {
        return true;
    }

    return false;
}
void BossHud::NotifyCollision(GameObject &other) {}