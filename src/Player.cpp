#include "Player.h"
#include "Sprite.h"
#include "Collider.h"
#include "Game.h"
#include "Camera.h"
#include "Damage.h"
#include "Timer.h"
#include "Sound.h"
#include "InputManager.h"
#include "TileMap.h"
#include "StageState.h"
#include "Text.h"

Player *Player::player;
Player::Player(GameObject &associated) : Being(associated, {100, 150}, 1, 5), combo(0), jumpCounter(0), invincible(false), invincibleTime(), jumpImpulse()
{
    player = this;
    associated.AddComponent(new Sprite(PLAYER_IDLE_FILE, associated, 6, 0.05f));
    associated.AddComponent(new Collider(associated, {64 / associated.box.w, 128 / associated.box.h}, {0, 20}));
}

Player::~Player()
{
    player = nullptr;
}

void Player::Start()
{
}

void Player::Update(float dt)
{
    // Useful objects
    StageState state = (StageState &)Game::GetInstance().GetCurrentState();
    TileMap *tileMap = state.GetTileMap();
    TileSet *tileSet = state.GetTileSet();
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");
    Collider *collider = (Collider *)associated.GetComponent("Collider");
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

    // check if in ground
    grounded = false;
    int tileLeftX = collider->box.x / tileSet->GetTileWidth();
    int tileRightX = (collider->box.x + collider->box.w) / tileSet->GetTileWidth();
    int tileBottomY = (collider->box.y + collider->box.h + 1) / tileSet->GetTileHeight();
    if (tileMap->IsSolid(tileLeftX, tileBottomY) or tileMap->IsSolid(tileRightX, tileBottomY))
    {
        grounded = true;
        jumpCounter = 0;
        speed.y = 0;
    }

    if (not grounded and (charState != DASHING))
        speed.y = speed.y + GRAVITY;
    int left = inputManager.IsKeyDown(A_KEY) ? 1 : 0;
    int right = inputManager.IsKeyDown(D_KEY) ? 1 : 0;
    int jump = inputManager.KeyPress(W_KEY) ? 1 : 0;
    int attack = inputManager.KeyPress(Z_KEY) ? 1 : 0;
    int dash = inputManager.KeyPress(SPACE_KEY) ? 1 : 0;

    float motionX;
    // State machine
    switch (charState)
    {
    case IDLE:
        // Actions

        // State change conditions
        if (inputManager.KeyPress(A_KEY) or inputManager.KeyPress(D_KEY))
        {
            Walk();
        }
        else if (jump)
        {
            Jump();
        }
        else if (not grounded)
        {
            Fall();
        }
        else if (attack)
        {
            Attack();
        }
        else if (dash)
        {
            Dash(dir);
        }
        break;

    case WALKING:
        // Actions
        // - update horizontal speed
        motionX = (right - left) * (MAX_SPEEDH * dt);
        if (motionX)
        {
            this->dir = right - left;
            moveX(motionX, collider->box, tileMap, tileSet);
        }

        // State change conditions
        if (not motionX)
        {
            Idle();
        }
        else if (jump)
        {
            Jump();
        }
        else if (not grounded)
        {
            Fall();
        }
        else if (attack)
        {
            Attack();
        }
        else if (dash)
        {
            Dash(dir);
        }
        break;

    case JUMPING:
        // Actions
        // - keep the impulse holding jump
        if (inputManager.IsKeyDown(W_KEY))
        {
            jumpImpulse.Update(dt);
            if (jumpImpulse.Get() <= 0.2f)
            {
                speed.y += -JUMP_HOLD / mass;
            }
        }

        // - update horizontal speed
        motionX = (right - left) * (MAX_SPEEDH * dt);
        if (motionX)
        {
            this->dir = right - left;
            moveX(motionX, collider->box, tileMap, tileSet);
        }

        // State change conditions
        if (speed.y > 0)
        {
            Fall();
        }
        else if (dash)
        {
            Dash(dir);
        }
        else if (jump and jumpCounter < 2)
        {
            Jump();
            sprite->SetFrame(0);
        }
        else if (attack)
        {
            Attack();
        }
        break;

    case FALLING:
        // Actions
        // - update horizontal speed
        motionX = (right - left) * (MAX_SPEEDH * dt);
        if (motionX)
        {
            this->dir = right - left;
            moveX(motionX, collider->box, tileMap, tileSet);
        }

        // State change conditions
        if (grounded)
        {
            if (inputManager.IsKeyDown(A_KEY) or inputManager.IsKeyDown(D_KEY))
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
            Jump();
        }
        break;

    case ATTACKING:
        // Actions
        // - create damage box
        if (sprite->GetCurrentFrame() == sprite->GetFrameCount() - 3)
        {
            GameObject *damage = new GameObject();
            damage->AddComponent(new Damage(*damage, 1, false, 0.15f));
            damage->box.w = 96;
            damage->box.h = 128;
            damage->box.y = collider->box.y;
            if (dir >= 0)
                damage->box.x = collider->box.x + collider->box.w;
            else
                damage->box.x = collider->box.x - damage->box.w;

            Game::GetInstance().GetCurrentState().AddObject(damage);
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
        motionX = (right - left) * (MAX_SPEEDH * dt);
        if (motionX)
        {
            this->dir = right - left;
            moveX(motionX, collider->box, tileMap, tileSet);
        }

        // Stage change conditions
        if (sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 1)
        {
            combo = 0;
            if (not grounded)
            {
                if (speed.y >= 0)
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
        motionX = speed.x * dt;
        moveX(motionX, collider->box, tileMap, tileSet);

        // State change conditions
        if (sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 1)
        {
            if (inputManager.IsKeyDown(W_KEY) and jumpCounter < 2)
            {
                Jump();
            }
            else if (inputManager.IsKeyDown(A_KEY) or inputManager.IsKeyDown(D_KEY))
            {
                Walk();
            }
            else if (not grounded)
            {
                Fall();
            }
            else
            {
                Idle();
            }
        }
        break;

    case HURT:
        // Actions

        // State change conditions
        if (sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 1)
        {
            if (inputManager.IsKeyDown(A_KEY) or inputManager.IsKeyDown(D_KEY))
            {
                Walk();
            }
            else if (not grounded)
            {
                if (speed.y >= 0)
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
            GameObject *gameOver = new GameObject();
            gameOver->box.y = Camera::pos.y + CAMERA_HEIGHT / 2;
            gameOver->box.x = Camera::pos.x + CAMERA_WIDTH / 2;
            gameOver->AddComponent(new Text(*gameOver, "assets/font/PeaberryBase.ttf", 150, Text::BLENDED, "YOU DIED", {255, 255, 255, SDL_ALPHA_TRANSPARENT}, 1));
            Game::GetInstance().GetCurrentState().AddObject(gameOver);

            associated.RequestDelete();
            Camera::Unfollow();
        }
        break;
    }

    // - update vertical speed (if not dashing)
    if (charState != DASHING)
    {
        moveY(speed.y * dt + (GRAVITY * (dt * dt)) / 2, collider->box, tileMap, tileSet);
    }
    // sprite direction
    sprite->SetDir(dir);
}

void Player::NotifyCollision(GameObject &other)
{
    if (other.GetComponent("Damage") != nullptr)
    {
        Damage *damage = (Damage *)other.GetComponent("Damage");

        if (damage->targetsPlayer and not(charState == DEAD or charState == DASHING) and not invincible)
        {
            Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");
            this->hp -= damage->GetDamage();

            this->charState = HURT;
            sprite->Change(PLAYER_HURT_FILE, 0.05, 4);
            Camera::TriggerShake(0.5f, {5.0f, 0});
            invincible = true;

            if (this->hp <= 0)
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
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");

    sprite->Change(PLAYER_IDLE_FILE, 0.05f, 6);
    charState = IDLE;
    speed.x = 0;
}

void Player::Attack()
{
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");

    sprite->Change(PLAYER_ATTACK1_FILE, 0.05f, 4);
    charState = ATTACKING;
}

void Player::Attack2()
{
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");

    sprite->Change(PLAYER_ATTACK2_FILE, 0.05f, 4);
    charState = ATTACKING;
}

void Player::Jump()
{
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");

    sprite->Change(PLAYER_JUMP_FILE, 0.05f, 3);
    speed.y = -JUMP_FORCE / mass;
    charState = JUMPING;
    jumpCounter++;
    jumpImpulse.Restart();
}

void Player::Fall()
{
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");

    sprite->Change(PLAYER_FALL_FILE, 0.05f, 5, 2);
    charState = FALLING;
}

void Player::Walk()
{
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");

    sprite->Change(PLAYER_RUN_FILE, 0.05f, 8);
    charState = WALKING;
}

void Player::Dash(int dir)
{
    Sprite *sprite = (Sprite *)associated.GetComponent("Sprite");

    sprite->Change(PLAYER_DASH_FILE, 0.04f, 7);
    charState = DASHING;
    this->speed.y = 0;
    if (dir >= 0)
        speed.x = MAX_SPEEDH * 3;
    else
        speed.x = -MAX_SPEEDH * 3;
}

bool Player::Is(std::string type)
{
    if (type.compare("Player") == 0)
    {
        return true;
    }

    return false;
}