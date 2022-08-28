#include "Player.h"
#include "Sprite.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "Camera.h"
#include "Damage.h"
#include "Timer.h"
#include "Sound.h"
#include "InputManager.h"
#include "TileMap.h"
#include "StageState.h"
#include "Text.h"
#include "RigidBody.h"

Player *Player::player;
Player::Player(GameObject &associated) : Being(associated, {100, 150}, 1, 5), combo(0), jumpCounter(0), invincible(false), canDash(false), invincibleTime(),
                                         wallSlideCooldown(), attackSound(new Sound(PLAYER_ATTACK_SOUND, associated)),
                                         jumpSound(new Sound(PLAYER_JUMP_SOUND, associated)), hurtSound(new Sound(PLAYER_HURT_SOUND, associated))
{
    player = this;
    associated.AddComponent(new Sprite(PLAYER_IDLE_FILE, associated, 6, 0.05f));
    associated.AddComponent(new Collider(associated, {60 / associated.box.w, 128 / associated.box.h}, {0, 20}));
    associated.AddComponent(this);
    associated.AddComponent(new RigidBody(associated));
}

Player::~Player()
{
    player = nullptr;
}

void Player::Start()
{
    this->sprite = static_cast<Sprite*>(associated.GetComponent("Sprite"));
    this->collider = static_cast<Collider*>(associated.GetComponent("Collider"));
    this->rigidBody = static_cast<RigidBody*>(associated.GetComponent("RigidBody"));
}

void Player::Update(float dt)
{
    // Useful objects
    TileMap *tileMap = static_cast<StageState&>(Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = static_cast<StageState&>(Game::GetInstance().GetCurrentState()).GetTileSet();
    InputManager &inputManager = InputManager::GetInstance();

    // remove invulnerability after 1 sec
    if (invincible)
    {
        invincibleTime.Update(dt);
        if (invincibleTime.Get() >= 1.0f)
        {
            invincible = false;
            invincibleTime.Restart();
        }
    }

    if (collider->IsGrounded())
    {
        jumpCounter = 0;
        rigidBody->velocity = Vec2(rigidBody->velocity.x, 0);
    }


    // check if knockbacking
    if(knockback)
    {   
        if(dir >= 0)
            moveX(-10, collider->box, tileMap, tileSet);
        else
            moveX(10, collider->box, tileMap, tileSet);
        knockbackTime.Update(dt);
        if(knockbackTime.Get() >= 0.1f)
        {
            knockback = false;
        }
    }

    // check if dash is in cooldown
    dashCooldown.Update(dt);
    if(dashCooldown.Get() >= 0.20f)
    {
        canDash = true;
    }

    //if (not grounded and (charState != DASHING))
        //rigidBody->velocity.y = rigidBody->velocity.y + GRAVITY;
    int left = inputManager.IsKeyDown(LEFT_ARROW_KEY) ? 1 : 0;
    int right = inputManager.IsKeyDown(RIGHT_ARROW_KEY) ? 1 : 0;
    int jump = inputManager.KeyPress(Z_KEY) ? 1 : 0;
    int attack = inputManager.KeyPress(X_KEY) ? 1 : 0;
    int dash = inputManager.KeyPress(C_KEY) and canDash ? 1 : 0;


    // State machine
    switch (charState)
    {
    case IDLE:
        // Actions
        if (left)
            this->dir = -1;
        else if (right)
            this->dir = 1;

        // State change conditions
        if (dash)
        {
            Dash(dir);
        }
        else if (attack)
        {
            Attack();
        }
        else if (right - left)
        {
            Walk();
        }
        else if (jump)
        {
            Jump();
        }
        else if (not collider->IsGrounded())
        {
            Fall();
        }
        break;

    case WALKING:
        // Actions
        // - update horizontal speed
        rigidBody->velocity.x = (right - left) * (MAX_SPEEDH);
        if (rigidBody->velocity.x)
        {
            this->dir = right - left;
        }

        // State change conditions
        if (dash)
        {
            Dash(dir);
        }
        else if (attack)
        {
            Attack();
        }
        else if (not rigidBody->velocity.x)
        {
            Idle();
        }
        else if (jump)
        {
            Jump();
        }
        else if (not collider->IsGrounded())
        {
            Fall();
        }
        break;

    case JUMPING:
        // Actions
        // - if release jump key, immediately fall
        if (inputManager.KeyRelease(Z_KEY) and rigidBody->velocity.y < 0)
            rigidBody->velocity = Vec2(rigidBody->velocity.x, rigidBody->velocity.y * 0.1f);

        // - update horizontal speed
        rigidBody->velocity.x = (right - left) * (MAX_SPEEDH);
        if (rigidBody->velocity.x)
        {
            this->dir = right - left;
        }

        // State change conditions
        if (rigidBody->velocity.y > 0)
        {
            Fall();
        }
        else if (collider->IsGrounded())
        {
            if (inputManager.IsKeyDown(LEFT_ARROW_KEY) or inputManager.IsKeyDown(RIGHT_ARROW_KEY))
            {
                Walk();
            }
            else
            {
                Idle();
            }
        }
        else if (dash)
        {
            Dash(dir);
        }
        else if (jump and jumpCounter < 2)
        {
            Jump2();
            sprite->SetFrame(0);
        }
        else if (attack)
        {
            Attack();
        }
        else if (IsOnWall())
        {
            WallSlide();
            jumpCounter = 0;
        }
        break;

    case FALLING:
        // Actions
        // - update horizontal speed
        rigidBody->velocity.x = (right - left) * (MAX_SPEEDH);
        if (rigidBody->velocity.x)
        {
            this->dir = right - left;
        }

        // State change conditions
        if (collider->IsGrounded())
        {
            if (inputManager.IsKeyDown(LEFT_ARROW_KEY) or inputManager.IsKeyDown(RIGHT_ARROW_KEY))
            {
                Walk();
            }
            else
            {
                Idle();
            }
        }
        else if (dash)
        {
            Dash(dir);
        }
        else if (jump and jumpCounter < 2)
        {
            Jump2();
            sprite->SetFrame(0);
        }
        else if (IsOnWall())
        {
            WallSlide();
            jumpCounter = 0;
        }
        break;

    case ATTACKING:
        // Actions
        // - create damage box and slash effect
        if (sprite->GetCurrentFrame() == sprite->GetFrameCount() - 3)
        {
            // Slash effect
            GameObject *slashEffect = new GameObject(0, associated.box.y);
            if (combo == 0)
            {
                Sprite *sprite = new Sprite("assets/img/playerslashfx.png", *slashEffect, 3, 0.03f, 0.09f);
                slashEffect->AddComponent(sprite);
                if (dir >= 0)
                {
                    slashEffect->box.x = associated.box.x + 10;
                }
                else
                {
                    slashEffect->box.x = associated.box.x - 10;
                    sprite->SetDir(-1);
                }
            }
            else
            {
                Sprite *sprite = new Sprite("assets/img/playerslashfx2.png", *slashEffect, 2, 0.03f, 0.06f);
                slashEffect->AddComponent(sprite);
                if (dir >= 0)
                {
                    slashEffect->box.x = associated.box.x + 10;
                }
                else
                {
                    slashEffect->box.x = associated.box.x - 10;
                    sprite->SetDir(-1);
                }
            }

            // damage box
            GameObject *damage = new GameObject(0, collider->box.y - (192 - collider->box.h) / 2);
            damage->AddComponent(new Damage(*damage, 1, false, 0.15f));
            damage->box.w = 64;
            damage->box.h = 192;
            if (dir >= 0)
                damage->box.x = collider->box.x + collider->box.w * 1.5;
            else
                damage->box.x = collider->box.x - damage->box.w - collider->box.w * 0.5;

            Game::GetInstance().GetCurrentState().AddObject(damage);
            Game::GetInstance().GetCurrentState().AddObject(slashEffect);
        }
        // - update combo
        if (attack and sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 2)
        {
            combo++;
            if (combo == 1)
            {
                Attack2();
            }
        }
        // - update horizontal speed
        rigidBody->velocity.x = (right - left) * (MAX_SPEEDH);
        if (rigidBody->velocity.x)
        {
            this->dir = right - left;
        }

        // Stage change conditions
        if (sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 1)
        {
            combo = 0;
            if (not collider->IsGrounded())
            {
                if (rigidBody->velocity.y >= 0)
                {
                    Fall();
                }
                else
                {
                    Jump();
                }
            }
            else if (dir)
            {
                Walk();
            }
            else
            {
                Idle();
            }
        }
        break;

    case DASHING:
        // Actions
        // - update horizontal speed

        // State change conditions
        if (sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 1)
        {   
            // start dash cooldown
            canDash = false;
            dashCooldown.Restart();

            if (inputManager.IsKeyDown(Z_KEY) and jumpCounter < 2)
            {
                Jump();
            }
            else if (inputManager.IsKeyDown(LEFT_ARROW_KEY) or inputManager.IsKeyDown(RIGHT_ARROW_KEY))
            {
                Walk();
            }
            else if (not collider->IsGrounded())
            {
                Fall();
            }
            else
            {
                Idle();
            }
        }
        break;

    case WALLSLIDING:
        // Actions
        this->rigidBody->velocity.y = 16;
        this->jumpCounter = 1;

        // State change conditions
        if (collider->IsGrounded())
        {
            if (inputManager.IsKeyDown(LEFT_ARROW_KEY) or inputManager.IsKeyDown(RIGHT_ARROW_KEY))
            {
                Walk();
            }
            else
            {
                Idle();
            }
            wallSlideCooldown.Restart();
        }
        else if (dash)
        {
            dir = right - left;
            Dash(dir);
            wallSlideCooldown.Restart();
        }
        else if (jump)
        {
            Jump();
            wallSlideCooldown.Restart();
        }
        else if (not collider->IsGrounded() and not IsOnWall())
        {
            Fall();
        }
        break;

    case HURT:
        // Actions

        // State change conditions
        if (sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 1)
        {
            if (inputManager.IsKeyDown(LEFT_ARROW_KEY) or inputManager.IsKeyDown(RIGHT_ARROW_KEY))
            {
                Walk();
            }
            else if (not collider->IsGrounded())
            {
                if (rigidBody->velocity.y >= 0)
                {
                    Fall();
                }
                else
                {
                    Jump();
                }
            }
            else
            {
                Idle();
            }
        }
        break;

    case DEAD:
        // Actions

        // State change conditions
        if (sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 1)
        {

            // Game over message
            GameObject *gameOver = new GameObject(Camera::pos.x + CAMERA_WIDTH / 2, Camera::pos.y + CAMERA_HEIGHT / 2);
            gameOver->AddComponent(new Text(*gameOver, "assets/font/PeaberryBase.ttf", 150, Text::BLENDED, "YOU DIED", {255, 255, 255, SDL_ALPHA_TRANSPARENT}, 1));
            Game::GetInstance().GetCurrentState().AddObject(gameOver);

            associated.RequestDelete();
            Camera::Unfollow();
        }
        break;
    }

    // - update vertical speed (if not dashing)
    //if (charState != DASHING)
    //{
        //moveY(rigidBody->velocity.y * dt + (GRAVITY * (dt * dt)) / 2, collider->box, tileMap, tileSet);
    //}
    // sprite direction
    sprite->SetDir(dir);
}

void Player::NotifyCollision(GameObject &other)
{
    if (other.GetComponent("Damage") != nullptr)
    {
        Damage *damage = static_cast<Damage*>(other.GetComponent("Damage"));

        if (damage->targetsPlayer and not(charState == DEAD or charState == DASHING) and not invincible)
        {
            GameData::playerHp -= damage->GetDamage();

            this->charState = HURT;
            sprite->Change(PLAYER_HURT_FILE, 0.05, 4);

            // play hurt sound
            hurtSound->Play();

            // add camera shake
            Camera::AddTrauma(0.6f);
            invincible = true;

            // knockback
            if (damage->GetBox().x > associated.box.x)
            {
                associated.box.x -= 5;
            }
            else
            {
                associated.box.x += 5;
            }

            if (GameData::playerHp <= 0)
            {
                this->charState = DEAD;
                sprite->Change(PLAYER_DEATH_FILE, 0.1, 11);
            }
        }
    }
    else if (other.GetComponent("Samurai") != nullptr)
    {
        if (not(charState == DEAD or charState == DASHING or invincible))
        {
            GameData::playerHp -= 1;

            this->charState = HURT;
            sprite->Change(PLAYER_HURT_FILE, 0.05, 4);

            // add camera shake
            Camera::AddTrauma(0.6f);
            invincible = true;

            if (GameData::playerHp <= 0)
            {
                this->charState = DEAD;
                sprite->Change(PLAYER_DEATH_FILE, 0.1, 11);
            }
        }
    }
}

void Player::Render() {}

void Player::Idle()
{
    sprite->Change(PLAYER_IDLE_FILE, 0.05f, 6);
    charState = IDLE;
    rigidBody->velocity.x = 0;
}

void Player::Attack()
{   
    sprite->Change(PLAYER_ATTACK1_FILE, 0.05f, 4);
    charState = ATTACKING;

    // play sound
    attackSound->Play();
}

void Player::Attack2()
{   
    sprite->Change(PLAYER_ATTACK2_FILE, 0.05f, 4);
    charState = ATTACKING;

    // play sound
    attackSound->Play();
}

void Player::Jump()
{
    sprite->Change(PLAYER_JUMP_FILE, 0.05f, 3);

    // apply force
    rigidBody->velocity.y = -JUMP_FORCE / mass;
    charState = JUMPING;
    jumpCounter++;
    // play sound
    jumpSound->Play();
}

void Player::Jump2()
{   
    sprite->Change(PLAYER_JUMP_FILE, 0.05f, 3);

    // apply force
    rigidBody->velocity.y = -JUMP_FORCE / mass;
    charState = JUMPING;
    jumpCounter++;
    // play sound
    jumpSound->Play();
}

void Player::Fall()
{
    sprite->Change(PLAYER_FALL_FILE, 0.05f, 5, 2);
    charState = FALLING;
}

void Player::Walk()
{
    sprite->Change(PLAYER_RUN_FILE, 0.05f, 8);
    charState = WALKING;
}

void Player::Dash(int dir)
{
    sprite->Change(PLAYER_DASH_FILE, 0.04f, 7);
    charState = DASHING;
    this->rigidBody->velocity.y = 0;
    if (dir >= 0)
        rigidBody->velocity.x = MAX_SPEEDH * 3;
    else
        rigidBody->velocity.x = -MAX_SPEEDH * 3;
}

void Player::WallSlide()
{
    sprite->Change(PLAYER_WALLSLIDE_FILE, 0.04f, 3);
    charState = WALLSLIDING;
}

bool Player::IsOnWall()
{
    TileMap *tileMap = static_cast<StageState&>(Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = static_cast<StageState&>(Game::GetInstance().GetCurrentState()).GetTileSet();

    if(dir >= 0)
    {
        return tileMap->IsSolid((collider->box.x + collider->box.w + 1) / tileSet->GetTileWidth(), collider->box.y / tileSet->GetTileHeight());
    }
    else
    {
        return tileMap->IsSolid((collider->box.x - 1) / tileSet->GetTileWidth(), collider->box.y / tileSet->GetTileHeight());
    }
}

bool Player::Is(std::string type)
{
    if (type.compare("Player") == 0)
    {
        return true;
    }

    return false;
}