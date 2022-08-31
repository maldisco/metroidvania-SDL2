#include "Skeleton.h"
#include "Collider.h"
#include "Game.h"
#include "Damage.h"
#include "StageState.h"
#include "Player.h"
#include "InputManager.h"
#include "Camera.h"
#include "Helpers.h"

Skeleton::Skeleton(GameObject &associated) : Component(associated), cooldown()
{
    associated.AddComponent(new Sprite(SKELETON_IDLE_FILE, associated, 4, 0.05f));
    associated.AddComponent(new Collider(associated, {64 / associated.box.w, 128 / associated.box.h}));
    associated.AddComponent(new RigidBody(associated));
    associated.AddComponent(new Health(associated, 3));
}

Skeleton::~Skeleton()
{
}

void Skeleton::Start()
{
    this->sprite = GetComponent<Sprite>();
    this->collider = GetComponent<Collider>();
    this->rigidBody = GetComponent<RigidBody>();
    this->health = GetComponent<Health>();
}

void Skeleton::Update(float dt)
{
    HandleDeath();

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
                        associated.direction = 1;
                    else
                        associated.direction = -1;
                }
                else if (Rect::Distance(Player::player->GetBox(), associated.box) <= 900)
                {
                    sprite->Change(SKELETON_RUN_FILE, 0.05f, 12);
                    charState = WALKING;
                    rigidBody->velocity = Vec2(SKELETON_SPEEDX * Helpers::Sign(associated.direction), rigidBody->velocity.y);
                }
            }
        }
        break;

    case WALKING:
        // Actions
        // - update timer (will walk for 1 sec)
        cooldown.Update(dt);

        // State change conditions
        if (Player::player != nullptr)
        {
            if (cooldown.Get() >= 3.0f)
            {
                // after 3 seconds enemy has a chance of idling or walking the other way
                if (rand() % 10 > 4)
                {
                    rigidBody->velocity = Vec2(rigidBody->velocity.x * -1, rigidBody->velocity.y);
                    associated.direction = Helpers::Sign(rigidBody->velocity.x);

                    cooldown.Restart();
                }
                else
                {
                    sprite->Change(SKELETON_IDLE_FILE, 0.05f, 4);
                    charState = IDLE;
                    rigidBody->velocity = Vec2(0, rigidBody->velocity.y);
                    cooldown.Restart();
                }
            }
            else if (Rect::Distance(Player::player->GetBox(), associated.box) > 900)
            {
                sprite->Change(SKELETON_IDLE_FILE, 0.05f, 4);
                rigidBody->velocity = Vec2(0, rigidBody->velocity.y);
                charState = IDLE;
                cooldown.Restart();
            }
            else if (Rect::Distance(Player::player->GetBox(), associated.box) < 64)
            {
                sprite->Change(SKELETON_ATTACK_FILE, 0.05f, 13);
                charState = ATTACKING;
                cooldown.Restart();
                if ((Player::player->GetBox().Center() - associated.box.Center()).x >= 0)
                    associated.direction = 1;
                else
                    associated.direction = -1;
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
            if (associated.direction >= 0)
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

        break;
    }
}

void Skeleton::HandleDeath()
{
    if(health->hp <= 0 and charState != DEAD)
    {
        this->charState = DEAD;
        sprite->Change(SKELETON_DEATH_FILE, 0.05, 13, 12);
        associated.layer = Enums::Corpse;
    }
}

void Skeleton::HandleDamage(Rect &box)
{
    Camera::AddTrauma(0.4f);

    if (this->charState != ATTACKING)
    {
        this->charState = HURT;
        sprite->Change(SKELETON_HURT_FILE, 0.05, 3);
    }

    // sleep for game feel
    SDL_Delay(20);
}

void Skeleton::NotifyCollision(GameObject &other)
{
}