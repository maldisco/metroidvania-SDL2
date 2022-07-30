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
    Npc(GameObject &associated, std::string file);
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject &other);

private:
    int dialogue;
};
#endif