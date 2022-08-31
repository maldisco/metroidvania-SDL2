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
#include "Animator.h"
#include "Helpers.h"
#include "Gravitypp.h"
#include "Physics.h"
#include "Skeleton.h"
#include "Transform.h"

Player *Player::player;
Player::Player(GameObject &associated) : Component(associated), combo(0), jumpCounter(0), invincible(false), canDash(false), invincibleTime(),
                                         wallSlideCooldown(), attackSound(new Sound(PLAYER_ATTACK_SOUND, associated)),
                                         jumpSound(new Sound(PLAYER_JUMP_SOUND, associated)), hurtSound(new Sound(PLAYER_HURT_SOUND, associated)),
                                         dashVelocity(1200), wallJumpForce({400, -1000}), attackRadius(64)
{
    player = this;
    Sprite *sprite = new Sprite(PLAYER_IDLE_FILE, associated, 6, 0.05f);
    associated.AddComponent(new Collider(associated, {60 / associated.box.w, 128 / associated.box.h}, {0, 20}));
    associated.AddComponent(new Transform(associated, Vec2(75, 0)));
    associated.AddComponent(this);
    associated.AddComponent(new Health(associated, 10));
    associated.AddComponent(new Animator(associated, sprite));
    associated.AddComponent(sprite);
    associated.AddComponent(new Gravitypp(associated, 1.5f));
    associated.AddComponent(new RigidBody(associated));
}

Player::~Player()
{
    player = nullptr;
}

void Player::Start()
{
    this->sprite = GetComponent<Sprite>();
    this->collider = GetComponent<Collider>();
    this->rigidBody = GetComponent<RigidBody>();
    this->animator = GetComponent<Animator>();
    this->health = GetComponent<Health>();
    this->attackPoint = GetComponent<Transform>();
    ConfigureAnimator();
}

void Player::Update(float dt)
{
    // Game event related (grounded, on wall)
    CheckWallSlideStop();
    ResetDash();

    // Fake coroutines
    HandleCoroutines(dt);

    // Input related
    JumpCut();
    GatherInput();

    CheckWallSlide();

    // Animations
    ApplyWalkingDirection();
    SetAnimator();

    // Physics
    ApplyHorizontalMovement();
    ApplyWallSlide();
    ApplyJump();
    ApplyDash();
    ApplyAttack();
}

void Player::Render()
{
    Physics::DrawCircle(Game::GetInstance().GetRenderer(), attackPoint->position.x - Camera::virtualPos.x, attackPoint->position.y - Camera::virtualPos.y, attackRadius);
}

void Player::GatherInput()
{
    InputManager &inputManager = InputManager::GetInstance();

    inputX = (inputManager.IsKeyDown(RIGHT_ARROW_KEY) ? 1 : 0) - (inputManager.IsKeyDown(LEFT_ARROW_KEY) ? 1 : 0);
    inputJump = inputManager.KeyPress(Z_KEY) ? 1 : 0;
    inputAttack = inputManager.KeyPress(X_KEY) ? 1 : 0;

    if (inputManager.KeyPress(C_KEY) and canDash)
    {
        isDashing = true;
        canDash = false;
        dashDir = Vec2(Helpers::Sign(associated.direction), 0);
    }
}

void Player::JumpCut()
{
    if (InputManager::GetInstance().KeyRelease(Z_KEY) and rigidBody->velocity.y < 0)
    {
        rigidBody->velocity = Vec2(rigidBody->velocity.x, rigidBody->velocity.y * 0.1f);
    }
}

void Player::CheckWallSlide()
{
    if (IsOnWall() and not collider->IsGrounded() and inputX != 0 and rigidBody->velocity.y > 0)
    {
        isWallSliding = true;
    }
}

void Player::CheckWallSlideStop()
{
    if (not IsOnWall() or collider->IsGrounded())
    {
        isWallSliding = false;
    }
}

void Player::ApplyWalkingDirection()
{
    if (inputX != 0)
    {
        associated.direction = inputX;
    }
}

void Player::ApplyHorizontalMovement()
{
    if (canMove and not isDashing)
    {
        rigidBody->velocity = Vec2(inputX * MAX_SPEEDH, rigidBody->velocity.y);
    }
}

void Player::ApplyWallSlide()
{
    if (isWallSliding and canMove)
    {
        rigidBody->velocity = Vec2(rigidBody->velocity.x, 16);
    }
}

void Player::ApplyDash()
{
    if (isDashing)
    {
        rigidBody->velocity = dashDir * dashVelocity;
    }
}

void Player::ApplyJump()
{
    if (collider->IsGrounded() and inputJump)
    {
        rigidBody->velocity = Vec2(rigidBody->velocity.x, -JUMP_FORCE);
        jumpSound->Play();
    }
    else if (isWallSliding and inputJump)
    {
        rigidBody->velocity = Vec2(wallJumpForce.x * -associated.direction, wallJumpForce.y);
        canMove = false;
        jumpSound->Play();
    }
}

void Player::ApplyAttack()
{
    if (inputAttack and not isAttacking and CanAttack())
    {
        isAttacking = true;
        auto hitEnemies = Physics::OverlapCircleAll(attackPoint->position, attackRadius, Enums::Enemy);
        for(auto enemy : hitEnemies)
        {
            enemy->GetComponent<IHittable>()->HandleDamage(associated.box);
            enemy->GetComponent<Health>()->Damage(1);
        }
        attackSound->Play();
    }
}

void Player::HandleCoroutines(float dt)
{
    if (isDashing)
    {
        dashCooldown.Update(dt);
        StopDashing();
    }

    if (not canMove)
    {
        pauseTimer.Update(dt);
        PauseControl();
    }

    if (isAttacking)
    {
        StopAttacking();
    }

    if (invincible)
    {
        invincibleTime.Update(dt);
        StopInvincibility();
    }
}

void Player::StopDashing()
{
    if (dashCooldown.Get() > 0.2f)
    {
        isDashing = false;
        dashCooldown.Restart();
    }
}

void Player::PauseControl()
{
    if (pauseTimer.Get() > 0.1f)
    {
        canMove = true;
        pauseTimer.Restart();
    }
}

void Player::StopAttacking()
{
    if (sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 1)
    {
        isAttacking = false;
    }
}

void Player::StopInvincibility()
{
    if (invincibleTime.Get() >= 1.0f)
    {
        invincible = false;
        invincibleTime.Restart();
    }
}

bool Player::CanAttack()
{
    return not(isWallSliding or isDashing);
}

void Player::ResetDash()
{
    if (collider->IsGrounded())
    {
        canDash = true;
    }
}

void Player::HandleDamage(Rect &box)
{
    animator->SetCondition("IsHurt", true);
    hurtSound->Play();
    Camera::AddTrauma(0.6f);
    invincible = true;
    associated.box.x += Helpers::Sign(associated.box.x - box.x) * 5;
}

void Player::NotifyCollision(GameObject &other)
{
    if (other.GetComponent<Damage>() != nullptr)
    {
        Damage *damage = static_cast<Damage *>(other.GetComponent<Damage>());

        if (damage->targetsPlayer and not(isDashing or GameData::playerHp <= 0 or invincible))
        {
            GameData::playerHp -= damage->GetDamage();

            animator->SetCondition("IsHurt", true);

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
        }
    }
}

bool Player::IsOnWall()
{
    TileMap *tileMap = static_cast<StageState &>(Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = static_cast<StageState &>(Game::GetInstance().GetCurrentState()).GetTileSet();

    return tileMap->IsSolid((collider->box.Center().x + ((collider->box.w/2 + 1)*Helpers::Sign(associated.direction))) / tileSet->GetTileWidth(), collider->box.y / tileSet->GetTileHeight());
}

void Player::SetAnimator()
{
    animator->SetCondition("IsWalking", inputX != 0);
    animator->SetCondition("IsStanding", inputX == 0);
    animator->SetCondition("IsGrounded", collider->IsGrounded());
    animator->SetCondition("IsOnAir", not collider->IsGrounded());
    animator->SetCondition("IsDashing", isDashing);
    animator->SetCondition("IsOnWall", IsOnWall());
    animator->SetCondition("IsNotOnWall", not IsOnWall());
    animator->SetCondition("IsRising", rigidBody->velocity.y < 0);
    animator->SetCondition("IsFalling", rigidBody->velocity.y >= 0);
    animator->SetCondition("IsAttacking", isAttacking);
    animator->SetCondition("IsDead", GameData::playerHp <= 0);
    animator->SetCondition("IsNotDead", GameData::playerHp > 0);
    animator->SetCondition("ExitAnim", sprite->GetCurrentFrame() >= sprite->GetFrameCount() - 1);
}

void Player::ConfigureAnimator()
{
    animator->AddSprite("idle", PLAYER_IDLE_FILE, 0.05f, 6);
    animator->AddSprite("run", PLAYER_RUN_FILE, 0.05f, 8);
    animator->AddSprite("jump", PLAYER_JUMP_FILE, 0.05f, 3),
    animator->AddSprite("attack", PLAYER_ATTACK1_FILE, 0.05f, 4);
    animator->AddSprite("attack2", PLAYER_ATTACK2_FILE, 0.05f, 4);
    animator->AddSprite("wallslide", PLAYER_WALLSLIDE_FILE, 0.04f, 3);
    animator->AddSprite("fall", PLAYER_FALL_FILE, 0.05f, 5, 2);
    animator->AddSprite("dash", PLAYER_DASH_FILE, 0.04f, 7);
    animator->AddSprite("hurt", PLAYER_HURT_FILE, 0.05, 4);
    animator->AddSprite("death", PLAYER_DEATH_FILE, 0.1f, 11, 10);

    animator->AddCondition("IsWalking");
    animator->AddCondition("IsStanding");
    animator->AddCondition("IsGrounded");
    animator->AddCondition("IsOnAir");
    animator->AddCondition("IsDashing");
    animator->AddCondition("IsOnWall");
    animator->AddCondition("IsNotOnWall");
    animator->AddCondition("IsRising");
    animator->AddCondition("IsFalling");
    animator->AddCondition("IsAttacking");
    animator->AddCondition("IsHurt");
    animator->AddCondition("IsDead");
    animator->AddCondition("IsNotDead");
    animator->AddCondition("ExitAnim");

    animator->AddConnection("idle", "run", {"IsWalking"});
    animator->AddConnection("idle", "jump", {"IsOnAir", "IsRising"});
    animator->AddConnection("idle", "attack", {"IsAttacking"});
    animator->AddConnection("idle", "fall", {"IsOnAir", "IsFalling"});
    animator->AddConnection("idle", "dash", {"IsDashing"});
    animator->AddConnection("idle", "hurt", {"IsHurt"});

    animator->AddConnection("run", "idle", {"IsStanding"});
    animator->AddConnection("run", "dash", {"IsDashing"});
    animator->AddConnection("run", "jump", {"IsOnAir", "IsRising"});
    animator->AddConnection("run", "fall", {"IsOnAir", "IsFalling"});
    animator->AddConnection("run", "attack", {"IsAttacking"});
    animator->AddConnection("run", "hurt", {"IsHurt"});

    animator->AddConnection("jump", "fall", {"IsOnAir", "IsFalling"});
    animator->AddConnection("jump", "attack", {"IsAttacking"});
    animator->AddConnection("jump", "dash", {"IsDashing"});
    animator->AddConnection("jump", "run", {"IsGrounded", "IsWalking"});
    animator->AddConnection("jump", "idle", {"IsGrounded", "IsStanding"});
    animator->AddConnection("jump", "hurt", {"IsHurt"});

    animator->AddConnection("fall", "idle", {"IsGrounded", "IsStanding"});
    animator->AddConnection("fall", "run", {"IsGrounded", "IsWalking"});
    animator->AddConnection("fall", "wallslide", {"IsOnWall", "IsOnAir", "IsWalking"});
    animator->AddConnection("fall", "dash", {"IsDashing"});
    animator->AddConnection("fall", "attack", {"IsAttacking"});
    animator->AddConnection("fall", "hurt", {"IsHurt"});

    animator->AddConnection("dash", "idle", {"ExitAnim", "IsStanding", "IsGrounded"});
    animator->AddConnection("dash", "run", {"ExitAnim", "IsWalking", "IsGrounded"});
    animator->AddConnection("dash", "fall", {"ExitAnim", "IsOnAir"});
    animator->AddConnection("dash", "wallslide", {"ExitAnim", "IsOnWall", "IsOnAir", "IsWalking"});

    animator->AddConnection("attack", "idle", {"ExitAnim", "IsGrounded", "IsStanding"});
    animator->AddConnection("attack", "run", {"ExitAnim", "IsGrounded", "IsWalking"});
    animator->AddConnection("attack", "fall", {"ExitAnim", "IsOnAir", "IsFalling"});
    animator->AddConnection("attack", "jump", {"ExitAnim", "IsOnAir", "IsRising"});
    animator->AddConnection("attack", "hurt", {"IsHurt"});

    animator->AddConnection("wallslide", "idle", {"IsGrounded"});
    animator->AddConnection("wallslide", "jump", {"IsNotOnWall", "IsOnAir", "IsRising"});
    animator->AddConnection("wallslide", "fall", {"IsNotOnWall", "IsOnAir", "IsFalling"});

    animator->AddConnection("hurt", "death", {"ExitAnim", "IsDead"});
    animator->AddConnection("hurt", "idle", {"ExitAnim", "IsNotDead"});
}