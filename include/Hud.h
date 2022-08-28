#ifndef HUD_H
#define HUD_H
#include "Component.h"
#include "Sprite.h"

#define HPBAR_BG "assets/img/hpbarbg.png"
#define HPBAR "assets/img/hpbar.png"
#define HPBAR_BORDER "assets/img/hpbarborder.png"

class Hud : public Component
{
public:
    Hud(GameObject &associated, int maxHp);
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject &other);

private:
    int maxHp;
    Sprite *hpbar;
};
#endif