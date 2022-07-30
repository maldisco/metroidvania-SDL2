#include "Npc.h"
#include "Sprite.h"
#include "Collider.h"
#include "Player.h"

Npc::Npc(GameObject &associated, std::string file) : Component(associated)
{
    associated.AddComponent(new Sprite(file, associated, 6, 0.1f));
    associated.AddComponent(new Collider(associated));
}

void Npc::Render()
{
}

void Npc::Update(float dt)
{
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");
    if(Player::player != nullptr)
    {
        if(Player::player->GetBox().Center().x < associated.box.Center().x)
        {
            sprite->SetDir(-1);
        }
        else
        {
            sprite->SetDir(1);
        }
    }
    
}

bool Npc::Is(std::string type)
{
    if (type.compare("Npc") == 0)
    {
        return true;
    }

    return false;
}
void Npc::NotifyCollision(GameObject &other) {}