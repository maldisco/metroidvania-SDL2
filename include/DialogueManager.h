#ifndef DIALOGUEMANAGER_H
#define DIALOGUEMANAGER_h
#include "Component.h"

class DialogueManager
{
public:
    bool inDialogue;

    DialogueManager(GameObject &associated);
    void Update(float dt);
    void Render();
    void NotifyCollision(GameObject &other);

private:
    std::queue<std::string> lines;
};
#endif