#include "include/Player.hpp"

Player::Player(Vector2f p_pos, SDL_Texture *idleTexture, SDL_Texture *walkTexture, SDL_Texture *jumpTexture, SDL_Texture *attackTexture, SDL_Texture *runningTexture,
               SDL_Texture *shieldTexture, SDL_Texture *hurtTexture, SDL_Texture *deadTexture, int frameCount, int frameWidth, int frameHeight)
    : Entity(p_pos, idleTexture, frameCount, frameWidth, frameHeight),
      idleTexture(idleTexture), walkTexture(walkTexture), jumpTexture(jumpTexture), attackTexture(attackTexture),
      runningTexture(runningTexture), shieldTexture(shieldTexture), hurtTexture(hurtTexture), deadTexture(deadTexture),
      currentState(PlayerState::Idle), jumpVelocity(0.0f), gravity(280.0f), isJumping(false), groundLevel(p_pos.y) {}

void Player::handleInput(const SDL_Event &event)
{
    if (isDead) return;  // ❌ Ignore input if dead
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_a:
            moveLeft = true;
            if (isRunning)
            {
                setState(PlayerState::Running);
            }
            else
            {
                setState(PlayerState::Walking);
            }
            break;
        case SDLK_d:
            moveRight = true;
            if (isRunning)
            {
                setState(PlayerState::Running);
            }
            else
            {
                setState(PlayerState::Walking);
            }
            break;
        case SDLK_SPACE:
            if (!isJumping)
            { // Only jump if not already jumping
                isJumping = true;
                jumpVelocity = -130.0f; // Initial upward velocity
                setState(PlayerState::Jumping);
            }
            break;
        case SDLK_LSHIFT:
            isRunning = true;
            if (moveLeft || moveRight)
            {
                setState(PlayerState::Running);
            }
            break;
        case SDLK_DOWN:
            setState(PlayerState::Hurt);
            hp -= 10;
            std::cout << "HP: " << hp << std::endl;
            if (hp <= 0)
            {
                setState(PlayerState::Dead); // ✅ Trigger death if HP reaches 0
            }
            break;
        }
    }

    switch (event.button.button)
    {
    case SDL_BUTTON_LEFT:
        if (currentState != PlayerState::Attacking)
        { // Only trigger once
            setState(PlayerState::Attacking);
        }
        break;
    case SDL_BUTTON_RIGHT:
        if (currentState != PlayerState::Shielding)
        {
            setState(PlayerState::Shielding);
        }
        break;
    }
    if (event.button.type == SDL_MOUSEBUTTONUP)
    {
        if (currentState == PlayerState::Shielding)
        {
            setState(PlayerState::Idle);
        }
    }
    if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_a:
            moveLeft = false;
            if (!moveRight)
            {
                setState(PlayerState::Idle);
            }
            else if (isRunning)
            {
                setState(PlayerState::Running);
            }
            else
            {
                setState(PlayerState::Walking);
            }
            break;
        case SDLK_d:
            moveRight = false;
            if (!moveLeft)
            {
                setState(PlayerState::Idle);
            }
            else if (isRunning)
            {
                setState(PlayerState::Running);
            }
            else
            {
                setState(PlayerState::Walking);
            }
            break;
        case SDLK_LSHIFT:
            isRunning = false;
            if (moveLeft || moveRight)
            {
                setState(PlayerState::Walking);
            }
            else
            {
                setState(PlayerState::Idle);
            }
            break;
        }

        if (!moveLeft && !moveRight && currentState == PlayerState::Walking)
        {
            setState(PlayerState::Idle);
        }
    }
}

void Player::setState(PlayerState newState)
{
    if (isDead)
        return; // ❌ Prevent any state changes after death
    if (currentState == newState)
        return;

    currentState = newState;
    switch (newState)
    {
    case PlayerState::Idle:
        setTexture(idleTexture);
        setFrameCount(6);
        break;
    case PlayerState::Walking:
        setTexture(walkTexture);
        setFrameCount(8);
        break;
    case PlayerState::Jumping:
        setTexture(jumpTexture);
        setFrameCount(12);
        resetAnimation();
        break;
    case PlayerState::Attacking:
        setTexture(attackTexture);
        setFrameCount(6);
        resetAnimation();
        break;
    case PlayerState::Running:
        setTexture(runningTexture);
        setFrameCount(8);
        break;
    case PlayerState::Shielding:
        setTexture(shieldTexture);
        setFrameCount(2);
        break;
    case PlayerState::Hurt:
        setTexture(hurtTexture);
        setFrameCount(2);
        break;
    case PlayerState::Dead:
        setTexture(deadTexture);
        setFrameCount(3);
        isDead = true; // ✅ Now the player is permanently "dead"
        break;
    }
}

void Player::updateMovement(float deltaTime)
{
    if (isDead) return;  // ❌ Stop movement if dead

    isMoving = moveLeft || moveRight;
    float currentSpeed = isRunning ? runSpeed : walkSpeed;

    if (isMoving)
    {
        if (moveLeft)
        {
            getPos().x -= currentSpeed;
        }
        if (moveRight)
        {
            getPos().x += currentSpeed;
        }
    }
    if (isJumping)
    {
        // Apply gravity
        jumpVelocity += gravity * deltaTime;
        getPos().y += jumpVelocity * deltaTime;

        // Check if the character has landed
        if (getPos().y >= groundLevel)
        {
            getPos().y = groundLevel; // Snap to ground level
            isJumping = false;
            jumpVelocity = 0.0f;
            setState(PlayerState::Idle); // Return to idle state after landing
        }
    }
}

void Player::updateAnimation(float deltaTime)
{
    if (isDead && animationFinished()) return;  // ❌ Stop animation updates after death
    Entity::updateAnimation(deltaTime);
    if (currentState == PlayerState::Dead) return; // ✅ Don't reset to Idle after death
    if (currentState == PlayerState::Dead && animationFinished())
    {
        // Stop updating the animation if the player is dead and the animation has finished
        return;
    }
    // If Jump or Attack finishes, return to Idle
    if ((currentState == PlayerState::Jumping || currentState == PlayerState::Attacking) || currentState == PlayerState::Hurt && animationFinished())
    {
        setState(PlayerState::Idle);
    }
}
