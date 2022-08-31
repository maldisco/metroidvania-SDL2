#include "BossHud.h"
#include "Sprite.h"
#include "CameraFollower.h"
#include "Camera.h"
#include "Player.h"

BossHud::BossHud(GameObject &associated, GameObject *boss, int maxHp) : Component(associated), boss(boss), maxHp(maxHp), hpbar(new Sprite(BOSS_HPBAR, associated))
{
    associated.AddComponent(new Sprite(BOSS_HPBAR_BG, associated));
    associated.AddComponent(hpbar);
    associated.AddComponent(new CameraFollower(associated, {CAMERA_WIDTH / 2 - 824 / 2, 860}));
}

void BossHud::Start()
{
    return;
    //this->status = GetComponent<Samurai>();
}

void BossHud::Update(float dt)
{   
    return;
    //if(status->GetHp() <= 0)
        //associated.RequestDelete();
    
    //hpbar->SetClipScale(static_cast<float>(status->GetHp()) / static_cast<float>(maxHp), hpbar->GetClipScale().y);
}