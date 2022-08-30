#include "Skeleton.h"
#include "Collider.h"
#include "Game.h"
#include "Damage.h"
#include "StageState.h"
#include "Player.h"
#include "InputManager.h"
#include "Camera.h"

Skeleton::Skeleton(GameObject &associated) : Being(associated, {300, 0}, 1.0f, 5), cooldown()
{
    associated.AddComponent(new Sprite(SKELETON_IDLE_FILE, associated, 4, 0.05f));
    associated.AddComponent(new Collider(associated, {64 / associated.box.w, 128 / associated.box.h}));
}

Skeleton::~Skeleton()
{
}

void Skeleton::Start()
{
    this->sprite = static_cast<Sprite*>(GetComponent<Sprite>());
    this->collider = static_cast<Collider*>(GetComponent<Collider>());
}

void Skeleton::Update(float dt)
{
    // Useful objects
    TileMap *tileMap = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileSet();
    InputManager &inputManager = InputManager::GetInstance();

    // check if knockbacking
    if(knockback)
    {   
        if(dir >= 0)
            moveX(-knockbackSpeed, collider->box, tileMap, tileSet);
        else
            moveX(knockbackSpeed, collider->box, tileMap, tileSet);
        knockbackSpeed = knockbackSpeed*0.7;
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
                    sprite->Change(SKELETON_ATTACK_FILE, 0.05f, 13);
                    charState = ATTACKING;
                    if ((Player::player->GetBox().Center() - associated.box.Center()).x >= 0)
                        dir = 1;
                    else
                        dir = -1;
                }
                else if (Rect::Distance(Player::player->GetBox(), associated.box) <= 900)
                {
                    sprite->Change(SKELETON_RUN_FILE, 0.05f, 12);
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

        // State change conditions
        if (Player::player != nullptr)
        {
            if (cooldown.Get() >= 3.0f)
            {
                // after 3 seconds enemy has a chanch of idling or walking the other way
                if (rand() % 10 > 4)
                {
                    speed.x = speed.x * -1;
                    cooldown.Restart();
                }
                else
                {
                    sprite->Change(SKELETON_IDLE_FILE, 0.05f, 4);
                    charState = IDLE;
                    speed.x = speed.x * -1;
                    cooldown.Restart();
                }
            }
            else if (Rect::Distance(Player::player->GetBox(), associated.box) > 900)
            {
                sprite->Change(SKELETON_IDLE_FILE, 0.05f, 4);
                charState = IDLE;
                cooldown.Restart();
            }
            else if (Rect::Distance(Player::player->GetBox(), associated.box) < 64)
            {
                sprite->Change(SKELETON_ATTACK_FILE, 0.05f, 13);
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
        if (sprite->GetCurrentFrame() == sprite->GetFrameCount() - 9)
        {
            GameObject *damage = new GameObject(0, collider->box.y);
            damage->AddComponent(new Damage(*damage, 1, true, 0.3f));
            damage->box.w = 96;
            damage->box.h = 128;
            if (dir >= 0)
                damage->box.x = collider->box.x + collider->box.w;
            else
                damage->box.x = collider->box.x - damage->box.w;

            Game::GetInstance().GetCurrentState().AddObject(damage);
        }

        // State change conditions
        if (sprite->GetCurrentFrame() == sprite->GetFrameCount() - 1)
        {
            sprite->Change(SKELETON_IDLE_FILE, 0.05f, 4);
            charState = IDLE;
        }
        break;

    case HURT:
        // Actions

        // State change conditions
        if (sprite->GetCurrentFrame() == sprite->GetFrameCount() - 1)
        {
            sprite->Change(SKELETON_IDLE_FILE, 0.05f, 4);
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

void Skeleton::NotifyCollision(GameObject &other)
{
    if (other.GetComponent<Damage>() != nullptr)
    {
        Damage *damage = static_cast<Damage*>(other.GetComponent<Damage>());
        if (not damage->targetsPlayer and not(charState == DEAD or charState == HURT))
        {
            this->hp -= damage->GetDamage();
            other.RequestDelete();

            // add camera shake
            Camera::AddTrauma(0.4f);

            // add knockback
            knockback = true;
            knockbackTime.Restart();
            knockbackSpeed = 30;

            if (this->charState != ATTACKING)
            {
                this->charState = HURT;
                sprite->Change(SKELETON_HURT_FILE, 0.05, 3);
            }

            if (this->hp <= 0)
            {
                this->charState = DEAD;
                sprite->Change(SKELETON_DEATH_FILE, 0.05, 13);
            }

            // add player knockback
            Player::player->knockback = true;

            // sleep for game feel
            SDL_Delay(20);
        }
    }
}