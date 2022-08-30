#include "Samurai.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "Damage.h"
#include "StageState.h"
#include "Player.h"
#include "InputManager.h"
#include "Camera.h"

Samurai::Samurai(GameObject &associated) : Being(associated, {200, 0}, 1.0f, 20), cooldown(), dashTime(),
dashPosLeft(128, 574), dashPosRight(1152, 574)
{
    associated.AddComponent(new Sprite(SAMURAI_IDLE_FILE, associated, 7, 0.05f));
    associated.AddComponent(new Collider(associated, {47 / associated.box.w, 85 / associated.box.h}, {0, 0}));
    dir = -1;
}

Samurai::~Samurai()
{
}

void Samurai::Start()
{
    this->sprite = static_cast<Sprite*>(associated.GetComponent<Sprite>());
    this->collider = static_cast<Collider*>(associated.GetComponent<Collider>());
}

void Samurai::Update(float dt)
{
    // Useful objects
    TileMap *tileMap = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = ((StageState &)Game::GetInstance().GetCurrentState()).GetTileSet();
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
                // 50% chance of dashing horizontally or vertically
                if(rand()%2)
                {
                    sprite->Change(SAMURAI_DASH_FILE, 0.1f, 10, 7);
                    charState = DASHING;
                    // go to the opposite side of the room 
                    // and dash to player
                    if(Vec2::Distance(Player::player->GetBox().Center(), dashPosLeft) > Vec2::Distance(Player::player->GetBox().Center(), dashPosRight))
                    {
                        associated.box.x = dashPosLeft.x;
                        associated.box.y = dashPosLeft.y;
                        dir = 1;
                    }
                    else
                    {
                        associated.box.x = dashPosRight.x;
                        associated.box.y = dashPosRight.y;
                        dir = -1;
                    }
                    speed.x = dir*(fabs(speed.x));
                }
                else
                {
                    // go right above player position
                    sprite->Change(SAMURAI_DASH_FILE, 0.2f, 10, 7);
                    charState = WALLSLIDING;
                    associated.box.Centered(Player::player->GetBox().Center());
                    associated.box.y -= 300;
                    associated.angleDeg = -90;
                    dir = -1;
                }
            }
        }
        break;

    case DASHING:
        // Actions

        // Only move after frame 6
        // charging before that
        if (sprite->GetCurrentFrame() >= 6)
        {
            moveX(speed.x * 5 * dt, collider->box, tileMap, tileSet);
            GameObject *damage = new GameObject(0, collider->box.y + 30);
            damage->AddComponent(new Damage(*damage, 2, true, 0.1f));
            damage->box.w = 116;
            damage->box.h = 35;
            if (speed.x >= 0)
                damage->box.x = collider->box.x + collider->box.w;
            else
                damage->box.x = collider->box.x - damage->box.w;

            Game::GetInstance().GetCurrentState().AddObject(damage);
        }

        // State change conditions
        dashTime.Update(dt);
        if (dashTime.Get() >= 2.0f)
        {
            sprite->Change(SAMURAI_IDLE_FILE, 0.05f, 7);
            charState = IDLE;
            dashTime.Restart();
        }
        break;

    case WALLSLIDING:
        // Actions

        // only move after frame 6
        // charging before that
        if (sprite->GetCurrentFrame() >= 6)
        {
            moveY(speed.y / 4 * dt, collider->box, tileMap, tileSet);
            GameObject *damage = new GameObject(collider->box.x - 30, collider->box.y + collider->box.h);
            damage->AddComponent(new Damage(*damage, 2, true, 0.1f));
            damage->box.w = 116;
            damage->box.h = 35;
            damage->angleDeg = associated.angleDeg;

            Game::GetInstance().GetCurrentState().AddObject(damage);
        } 
        else // follow player X position while charging
        {
            associated.box.Centered(Player::player->GetBox().Center());
            associated.box.y -= 300;
        }

        // State change conditions
        if (grounded)
        {
            sprite->Change(SAMURAI_IDLE_FILE, 0.05f, 7);
            charState = IDLE;
            associated.angleDeg = 0;
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
    if (charState != WALLSLIDING)
        moveY(speed.y * dt + (GRAVITY * (dt * dt)) / 2, collider->box, tileMap, tileSet);

    sprite->SetDir(dir);
}

void Samurai::NotifyCollision(GameObject &other)
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
            if (this->charState != DASHING)
            {
                this->charState = HURT;
                sprite->Change(SAMURAI_HURT_FILE, 0.02f, 5);
                associated.angleDeg = 0;
            }

            if (this->hp <= 0)
            {
                this->charState = DEAD;
                sprite->Change(SAMURAI_DEAD_FILE, 0.1f, 17);
                associated.angleDeg = 0;
                // set samurai as slain
                GameData::samuraiSlain = true;
            }

            // add player knockback
            Player::player->knockback = true;

            // sleep for game feel
            SDL_Delay(20);
        }
    }
}