#include "DialogueManager.h"
#include "InputManager.h"
#include "Player.h"

DialogueManager *DialogueManager::Instance;
DialogueManager::DialogueManager(GameObject &associated, GameObject &box, GameObject &text, GameObject &name) : Component(associated), dialogueBox(box),
dialogueText(text), dialogueName(name) {}


void DialogueManager::Start()
{
    this->dialogue = dialogueText.GetComponent<Text>();
    this->name = dialogueName.GetComponent<Text>();
    this->dialogueBox.SetActive(false);
    this->dialogueText.SetActive(false);
    this->dialogueName.SetActive(false);
    this->Instance = this;
}

void DialogueManager::Update(float dt)
{
    if(InputManager::GetInstance().KeyPress(SPACE_KEY))
    {
        if(not lines.empty())
        {
            this->dialogue->SetText(lines.front());
            lines.pop();
        }
        else
        {
            EndDialogue();
        }
    }
}

void DialogueManager::StartDialogue(std::vector<std::string> dialogueLines, std::string name)
{
    for(auto line : dialogueLines)
    {
        lines.push(line);
    }

    this->name->SetText(name);
    this->dialogue->SetText(lines.front());
    lines.pop();

    this->dialogueBox.SetActive(true);
    this->dialogueText.SetActive(true);
    this->dialogueName.SetActive(true);

    inDialogue = true;
    Player::player->enabled = false;
}

void DialogueManager::EndDialogue()
{
    this->dialogueBox.SetActive(false);
    this->dialogueText.SetActive(false);
    this->dialogueName.SetActive(false);

    inDialogue = false;
    Player::player->enabled = true;
}