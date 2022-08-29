#ifndef PLAYER_H
#define PLAYER_H

#define MAX_SPEEDH 400
#define JUMP_FORCE 1000
#define JUMP_HOLD 175

#define PLAYER_JUMP_FILE "assets/img/warrior-jump.png"
#define PLAYER_RUN_FILE "assets/img/warrior-run.png"
#define PLAYER_IDLE_FILE "assets/img/warrior-idle.png"
#define PLAYER_FALL_FILE "assets/img/warrior-fall.png"
#define PLAYER_ATTACK1_FILE "assets/img/warrior-attack1.png"
#define PLAYER_ATTACK2_FILE "assets/img/warrior-attack2.png"
#define PLAYER_CROUCH_FILE "assets/img/warrior-crouch.png"
#define PLAYER_DASH_FILE "assets/img/warrior-dash.png"
#define PLAYER_DASHATTACK_FILE "assets/img/warrior-dash-attack.png"
#define PLAYER_DEATH_FILE "assets/img/warrior-death.png"
#define PLAYER_SLIDE_FILE "assets/img/warrior-slide.png"
#define PLAYER_HURT_FILE "assets/img/warrior-hurt.png"
#define PLAYER_WALLSLIDE_FILE "assets/img/warrior-wallslide.png"

#define PLAYER_ATTACK_SOUND "assets/audio/attack2.wav"
#define PLAYER_JUMP_SOUND "assets/audio/jump1.wav"
#define PLAYER_HURT_SOUND "assets/audio/damaged2.wav"

#include "Being.h"
#include "Sound.h"
#include "RigidBody.h"
#include "Animator.h"

class Player : public Being
{
public:
    /**
     * @brief Construct a new Player object
     *
     * @param associated
     */
    Player(GameObject &associated);
    ~Player();

    void Start();
    void OldUpdate(float dt);
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject &other);

    void ConfigureAnimator();
    void SetAnimator();
    void GatherInput();
    void JumpCut();
    void CheckWallSlide();
    void CheckWallSlideStop();
    void ResetDash();
    void ApplyWalkingDirection();
    void ApplyHorizontalMovement();
    void ApplyWallSlide();
    void ApplyJump();
    void ApplyDash();
    void ApplyAttack();
    bool CanAttack();
    void StopAttacking();
    void StopDashing();
    void PauseControl();
    void HandleCoroutines(float dt);

    bool IsOnWall();

    void Idle();
    void Attack();
    void Attack2();
    void Jump();
    void Jump2();
    void Walk();
    void Dash(int dir);
    void WallSlide();
    void Fall();

    static Player *player;

private:
    int combo, jumpCounter, inputX, inputJump, inputAttack, inputDash;
    bool invincible, canDash, isWallSliding, isDashing, isAttacking, canMove;
    float dashVelocity;
    Vec2 dashDir, wallJumpForce;
    Timer invincibleTime, pauseTimer, wallSlideCooldown, dashCooldown;
    Sound *attackSound, *jumpSound, *hurtSound;
    Sprite *sprite;
    Collider *collider;
    RigidBody *rigidBody;
    Animator *animator;
};
#endif