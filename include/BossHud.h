#ifndef BOSSHUD_H
#define BOSSHUD_H
#include "Component.h"
#include "Sprite.h"
#include "Being.h"

#define BOSS_HPBAR_BG "assets/img/bossbarbg.png"
#define BOSS_HPBAR "assets/img/bossbar.png"
#define BOSS_HPBAR_BORDER "assets/img/bossbarborder.png"

class BossHud : public Component
{
public:
    BossHud(GameObject &associated, GameObject *boss, int maxHp = 10);
    void Start();
    void Update(float dt);

private:
    int maxHp;
    Sprite *hpbar;
    GameObject *boss;
    Being *status;
};
#endif