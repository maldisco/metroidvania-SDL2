#ifndef NPC_H
#define NPC_H
#include "Component.h"

class Npc : public Component
{
public:
    /**
     * @brief Construct a new Npc object
     * 
     * @param associated 
     * @param file 
     */
    Npc(GameObject &associated, std::string sprite, std::string dialogues);
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject &other);
    void LoadDialogue(std::string file);

private:
    int line;
    std::vector<std::string> dialogueLines;
};
#endif