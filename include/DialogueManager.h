#ifndef DIALOGUEMANAGER_H
#define DIALOGUEMANAGER_H

#include "Component.h"
#include "Text.h"

class DialogueManager : public Component
{
public:
    bool inDialogue;
    static DialogueManager *Instance;

    DialogueManager(GameObject &associated, GameObject &box, GameObject &text, GameObject &name);
    void Start();
    void Update(float dt);
    void StartDialogue(std::vector<std::string> dialogueLines, std::string npcName);
    void EndDialogue();

private:
    std::queue<std::string> lines;
    GameObject &dialogueBox, &dialogueText, &dialogueName;
    Text *dialogue;
    Text *name;
};
#endif