#include "Npc.h"
#include "Sprite.h"
#include "Collider.h"
#include "Player.h"
#include "InputManager.h"
#include "Text.h"
#include "Game.h"
#include "GameData.h"

Npc::Npc(GameObject &associated, std::string sprite, std::string dialogue) : Component(associated)
{
    associated.AddComponent(new Sprite(sprite, associated, 6, 0.1f));
    associated.AddComponent(new Collider(associated));
    LoadDialogue(dialogue);
}

void Npc::Start()
{
    this->sprite = GetComponent<Sprite>();
}

void Npc::LoadDialogue(std::string file)
{
    std::fstream dialogue;
    std::vector<int> dims;

    dialogue.open(file);
    if (dialogue.is_open())
    {
        std::string line;
        while (getline(dialogue, line))
        {
            dialogueLines.push_back(line);
        }
    }
}

void Npc::Update(float dt)
{
    Turn();
}

void Npc::Turn()
{
    if(Player::player != nullptr)
    {
        if(Player::player->GetBox().Center().x < associated.box.Center().x)
        {
            associated.direction = -1;
        }
        else
        {
            associated.direction = 1;
        }
    }
}

void Npc::NotifyCollision(GameObject &other)
{
    if (other.GetComponent<Player>() != nullptr)
    {
        if(InputManager::GetInstance().KeyPress(E_KEY))
        {
            GameObject *dialogue = new GameObject();
            dialogue->box.x = associated.box.Center().x;
            dialogue->box.y = associated.box.y - 50;
            std::string line = dialogueLines.at(GameData::bluewitchLine);
            if(GameData::samuraiSlain)
                line = "Thanks! My brother was trying to kill that skeleton for a long time";
            dialogue->AddComponent(new Text(*dialogue, "assets/font/PeaberryBase.ttf", 30, Text::BLENDED, line, {0, 0, 0, SDL_ALPHA_OPAQUE}, 0, 3));
            Game::GetInstance().GetCurrentState().AddObject(dialogue);
            if(GameData::bluewitchLine < dialogueLines.size()-1)
                GameData::bluewitchLine++;
        }
    }
}