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
    Hud(GameObject &associated, Sprite *sprite);
    void Update(float dt);

    void UpdateBar();

private:
    Sprite *hpbar;
};
#endif