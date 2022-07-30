#include "Samurai.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "Damage.h"
#include "StageState.h"
#include "Player.h"
#include "InputManager.h"
#include "Camera.h"

Samurai::Samurai(GameObject &associated) : Being(associated, {300, 0}, 1.0f, 10), cooldown(), hitCooldown(), dashCooldown(), dashTime()
{
    associated.AddComponent(new Sprite(SAMURAI_IDLE_FILE, associated, 7, 0.05f));
    associated.AddComponent(new Collider(associated, {47 / associated.box.w, 85 / associated.box.h}, {0, 0}));
}

Samurai::~Samurai()
{
}

void Samurai::Start()
{
}

void Samurai::Update(float dt)
{
    // Useful objects
    TileMap *tileMap = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileSet();
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");
    Collider *collider = (Collider *)associated.GetComponent("Collider");
    InputManager &inputManager = InputManager::GetInstance();

    // check if in ground
    bool grounded = false;
    int tileLeftX = collider->box.x / tileSet->GetTileWidth();
    int tileRightX = (collider->box.x + collider->box.w) / tileSet->GetTileWidth();
    int tileBottomY = (collider->box.y + collider->box.h + 1) / tileSet->GetTileHeight();
    if (tileMap->IsSolid(tileLeftX, tileBottomY) or tileMap->IsSolid(tileRightX, tileBottomY))
    {
        grounded = true;
        speed.y = 0;
    }

    // check if can dash
    bool dash = false;
    dashCooldown.Update(dt);
    if (dashCooldown.Get() >= 5.0f)
        dash = true;

    bool attack = false;
    hitCooldown.Update(dt);
    if(Player::player != nullptr){
        if(hitCooldown.Get() >= 3.0f and Rect::Distance(Player::player->GetBox(), associated.box) <= 64)
            attack = true;
    }

    speed.y = speed.y + GRAVITY;
    switch (charState)
    {
    case IDLE:
        // Actions
        cooldown.Update(dt);

        // State change conditions
        if (cooldown.Get() >= 1.0f)
        {
            cooldown.Restart();
            if (Player::player != nullptr)
            {
                if (dash)
                {
                    sprite->Change(SAMURAI_DASH_FILE, 0.05f, 10, 7);
                    charState = DASHING;
                    if ((Player::player->GetBox().Center() - associated.box.Center()).x >= 0)
                        dir = 1;
                    else
                        dir = -1;
                }
                else if (attack)
                {
                    sprite->Change(SAMURAI_ATTACK_FILE, 0.05f, 6);
                    charState = ATTACKING;
                    if ((Player::player->GetBox().Center() - associated.box.Center()).x >= 0)
                        dir = 1;
                    else
                        dir = -1;
                }
                else if (Rect::Distance(Player::player->GetBox(), associated.box) <= 1800)
                {
                    sprite->Change(SAMURAI_MOVE_FILE, 0.05f, 6);
                    charState = WALKING;
                }
            }
        }
        break;

    case WALKING:
        // Actions
        // - update timer (will walk for 1 sec)
        cooldown.Update(dt);

        // - update horizontal speed
        if (speed.x >= 0)
            dir = 1;
        else
            dir = -1;

        if(Player::player != nullptr)
        {
            if ((Player::player->GetBox().Center() - associated.box.Center()).x >= 0)
                speed.x = fabs(speed.x);
            else
                speed.x = -fabs(speed.x);
            moveX(speed.x * dt, collider->box, tileMap, tileSet);
        }        

        // State change conditions
        if (Player::player != nullptr)
        {
            if (dash)
            {
                sprite->Change(SAMURAI_DASH_FILE, 0.05f, 10, 7);
                charState = DASHING;
                if ((Player::player->GetBox().Center() - associated.box.Center()).x >= 0)
                    dir = 1;
                else
                    dir = -1;
            }
            else if (attack)
                {
                    sprite->Change(SAMURAI_ATTACK_FILE, 0.05f, 6);
                    charState = ATTACKING;
                    if ((Player::player->GetBox().Center() - associated.box.Center()).x >= 0)
                        dir = 1;
                    else
                        dir = -1;
                }
            else if (cooldown.Get() >= 3.0f)
            {
                cooldown.Restart();
                // after 3 seconds enemy has a chance of idling or walking the other way
                if (rand() % 10 > 6)
                {
                    sprite->Change(SAMURAI_IDLE_FILE, 0.05f, 7);
                    charState = IDLE;
                }
            }
            else if (Rect::Distance(Player::player->GetBox(), associated.box) > 1800)
            {
                sprite->Change(SAMURAI_IDLE_FILE, 0.05f, 7);
                charState = IDLE;
                cooldown.Restart();
            }
        }
        break;

    case ATTACKING:
        // Actions
        if (sprite->GetCurrentFrame() == 1)
        {
            GameObject *damage = new GameObject();
            damage->AddComponent(new Damage(*damage, 2, true, 0.3f));
            damage->box.w = 16;
            damage->box.h = 85;
            damage->box.y = collider->box.y;
            if (dir >= 0)
                damage->box.x = collider->box.x + collider->box.w;
            else
                damage->box.x = collider->box.x - damage->box.w;

            Game::GetInstance().GetCurrentState().AddObject(damage);
        }

        // State change conditions  
        if(sprite->GetCurrentFrame() >= sprite->GetFrameCount()-1)
        {
            sprite->Change(SAMURAI_IDLE_FILE, 0.05f, 7);
            charState = IDLE;
            cooldown.Restart();
        }
        break;


    case DASHING:
        // Actions
        if(sprite->GetCurrentFrame() >= 6)
            moveX(speed.x * 3 * dt, collider->box, tileMap, tileSet);

        // State change conditions
        dashTime.Update(dt);
        if (dashTime.Get() >= 2.0f)
        {
            sprite->Change(SAMURAI_IDLE_FILE, 0.05f, 7);
            charState = IDLE;
            dashCooldown.Restart();
            dashTime.Restart();
        }
        break;

    case HURT:
        // Actions

        // State change conditions
        if (sprite->GetCurrentFrame() == sprite->GetFrameCount() - 1)
        {
            sprite->Change(SAMURAI_IDLE_FILE, 0.05f, 7);
            charState = IDLE;
        }
        break;

    case DEAD:
        // Actions

        // State change conditions
        if (sprite->GetCurrentFrame() == sprite->GetFrameCount() - 1)
        {
            associated.RequestDelete();
            
        }
        break;
    }

    // - update vertical speed (all states do)
    moveY(speed.y * dt + (GRAVITY * (dt * dt)) / 2, collider->box, tileMap, tileSet);

    sprite->SetDir(dir);
}

void Samurai::Render()
{
}

bool Samurai::Is(std::string type)
{
    if (type.compare("Samurai") == 0)
    {
        return true;
    }

    return false;
}

void Samurai::NotifyCollision(GameObject &other)
{
    if (other.GetComponent("Damage") != nullptr)
    {
        Damage *damage = (Damage *)other.GetComponent("Damage");
        if (not damage->targetsPlayer and not(charState == DEAD or charState == HURT))
        {
            Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");
            this->hp -= damage->GetDamage();
            other.RequestDelete();

            Camera::TriggerShake(0.4f, {3.0f, 0});
            if (this->charState != DASHING)
            {
                this->charState = HURT;
                sprite->Change(SAMURAI_HURT_FILE, 0.05f, 5);
            }

            if (this->hp <= 0)
            {
                this->charState = DEAD;
                sprite->Change(SAMURAI_DEAD_FILE, 0.05f, 17);
                GameData::samuraiSlain = true;
            }
        }
    }
}