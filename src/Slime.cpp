#include "Slime.h"
#include "Collider.h"
#include "Game.h"
#include "Damage.h"
#include "StageState.h"
#include "Player.h"
#include "InputManager.h"
#include "Camera.h"

Slime::Slime(GameObject &associated) : Being(associated, {200, 0}, 1.0f, 3), cooldown()
{
    associated.AddComponent(new Sprite(SLIME_IDLE_FILE, associated, 4, 0.1f));
    associated.AddComponent(new Collider(associated, {111 / associated.box.w, 75 / associated.box.h}, {0, 23}));
}

Slime::~Slime()
{
}

void Slime::Start()
{
    this->sprite = static_cast<Sprite*>(associated.GetComponent("Sprite"));
    this->collider = static_cast<Collider*>(associated.GetComponent("Collider"));
}

void Slime::Update(float dt)
{
    // Useful objects
    TileMap *tileMap = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileSet();
    InputManager &inputManager = InputManager::GetInstance();

    // check if knockbacking
    if(knockback)
    {   
        if(dir >= 0)
            moveX(-30, collider->box, tileMap, tileSet);
        else
            moveX(30, collider->box, tileMap, tileSet);
        knockbackTime.Update(dt);
        if(knockbackTime.Get() >= 0.1f)
        {
            knockback = false;
        }
    }

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
                if (Rect::Distance(Player::player->GetBox(), associated.box) <= 64)
                {
                    sprite->Change(SLIME_ATTACK_FILE, 0.1f, 5);
                    charState = ATTACKING;
                    if ((Player::player->GetBox().Center() - associated.box.Center()).x >= 0)
                        dir = 1;
                    else
                        dir = -1;
                }
                else if (Rect::Distance(Player::player->GetBox(), associated.box) <= 900)
                {
                    sprite->Change(SLIME_MOVE_FILE, 0.1f, 4);
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
        moveX(speed.x * dt, collider->box, tileMap, tileSet);

        // - update direction
        if (cooldown.Get() >= 3.0f)
        {
            speed.x = speed.x * -1;
            cooldown.Restart();
        }

        // State change conditions
        if (Player::player != nullptr)
        {
            if (Rect::Distance(Player::player->GetBox(), associated.box) > 900)
            {
                sprite->Change(SLIME_IDLE_FILE, 0.1f, 4);
                charState = IDLE;
                cooldown.Restart();
            }
            else if (Rect::Distance(Player::player->GetBox(), associated.box) < 64)
            {
                sprite->Change(SLIME_ATTACK_FILE, 0.1f, 5);
                charState = ATTACKING;
                cooldown.Restart();
                if ((Player::player->GetBox().Center() - associated.box.Center()).x >= 0)
                    dir = 1;
                else
                    dir = -1;
            }
        }
        break;

    case ATTACKING:
        // Actions
        if (sprite->GetCurrentFrame() == 2)
        {
            GameObject *damage = new GameObject(0, collider->box.y);
            damage->AddComponent(new Damage(*damage, 1, true, 0.3f));
            damage->box.w = 96;
            damage->box.h = 96;
            if (dir >= 0)
                damage->box.x = collider->box.x + collider->box.w;
            else
                damage->box.x = collider->box.x - damage->box.w;

            Game::GetInstance().GetCurrentState().AddObject(damage);
        }

        // State change conditions
        if (sprite->GetCurrentFrame() == sprite->GetFrameCount() - 1)
        {
            sprite->Change(SLIME_IDLE_FILE, 0.1f, 4);
            charState = IDLE;
        }
        break;

    case HURT:
        // Actions

        // State change conditions
        if (sprite->GetCurrentFrame() == sprite->GetFrameCount() - 1)
        {
            sprite->Change(SLIME_IDLE_FILE, 0.1f, 4);
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

void Slime::Render()
{
}

bool Slime::Is(std::string type)
{
    if (type.compare("Slime") == 0)
    {
        return true;
    }

    return false;
}

void Slime::NotifyCollision(GameObject &other)
{
    if (other.GetComponent("Damage") != nullptr)
    {
        Damage *damage = static_cast<Damage*>(other.GetComponent("Damage"));
        if (not damage->targetsPlayer and not(charState == DEAD or charState == HURT))
        {
            this->hp -= damage->GetDamage();
            other.RequestDelete();
            
            // add camera shake            
            Camera::AddTrauma(0.4f);

            // add knockback
            knockback = true;
            knockbackTime.Restart();

            if (this->charState != ATTACKING)
            {
                this->charState = HURT;
                sprite->Change(SLIME_HURT_FILE, 0.05, 4);
            }

            if (this->hp <= 0)
            {
                this->charState = DEAD;
                sprite->Change(SLIME_DEATH_FILE, 0.05, 5);
            }

            // add player knockback
            Player::player->knockback = true;

            // sleep for game feel
            SDL_Delay(20);
        }
    }
}