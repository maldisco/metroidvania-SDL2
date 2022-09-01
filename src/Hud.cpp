#include "Hud.h"
#include "GameData.h"
#include "Player.h"

Hud::Hud(GameObject &associated, Sprite *hpBar) : Component(associated), hpbar(hpBar)
{
    // Decrease or increase hp bar
    UpdateBar();
}

void Hud::Update(float dt)
{
    UpdateBar();
}

void Hud::UpdateBar()
{
    int maxHp = Player::player->GetComponent<Health>()->maxHp;
    int hp = Player::player->GetComponent<Health>()->hp;

    hpbar->SetClipScale(static_cast<float>(hp) / static_cast<float>(maxHp), hpbar->GetClipScale().y);
}