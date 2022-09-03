#include "Npc.h"
#include "Sprite.h"
#include "Collider.h"
#include "Player.h"
#include "InputManager.h"
#include "Text.h"
#include "Game.h"
#include "GameData.h"
#include "DialogueManager.h"

Npc::Npc(GameObject &associated, std::string dialogue) : Component(associated)
{
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
        if(InputManager::GetInstance().KeyPress(E_KEY) and not DialogueManager::Instance->inDialogue)
        {
            DialogueManager::Instance->StartDialogue(this->dialogueLines, "Magda");
        }
    }
}