#include "include/Player.hpp"

Player::Player(Vector2f p_pos, SDL_Texture* idleTexture, SDL_Texture* walkTexture, SDL_Texture* jumpTexture, SDL_Texture* attackTexture, SDL_Texture* runningTexture, 
    SDL_Texture* shieldTexture,  int frameCount, int frameWidth, int frameHeight)
: Entity(p_pos, idleTexture, frameCount, frameWidth, frameHeight), 
idleTexture(idleTexture), walkTexture(walkTexture), jumpTexture(jumpTexture), attackTexture(attackTexture),
runningTexture(runningTexture), shieldTexture(shieldTexture), 
currentState(PlayerState::Idle) {}


void Player::handleInput(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_a:
                moveLeft = true;
                if (isRunning) {
                    setState(PlayerState::Running);
                } else {
                    setState(PlayerState::Walking);
                }
                break;
            case SDLK_d:
                moveRight = true;
                if (isRunning) {
                    setState(PlayerState::Running);
                } else {
                    setState(PlayerState::Walking);
                }
                break;
            case SDLK_SPACE:
                if (currentState != PlayerState::Jumping) { // Only trigger once
                    setState(PlayerState::Jumping);
                }
                break;
            case SDLK_LSHIFT:
                isRunning = true;
                if (moveLeft || moveRight) {
                    setState(PlayerState::Running);
                }
                break;
        }

        
    }
    switch (event.button.button)
    {
    case SDL_BUTTON_LEFT:
    if (currentState != PlayerState::Attacking) { // Only trigger once
        setState(PlayerState::Attacking);
    }
        break;
    case SDL_BUTTON_RIGHT:
    if(currentState != PlayerState::Shielding){ 
        setState(PlayerState::Shielding);
    }
        break;
    }
    if(event.button.type == SDL_MOUSEBUTTONUP){
        if(currentState == PlayerState::Shielding){
            setState(PlayerState::Idle);
        }
    }
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_a:
                moveLeft = false;
                if (!moveRight) {
                    setState(PlayerState::Idle);
                } else if (isRunning) {
                    setState(PlayerState::Running);
                } else {
                    setState(PlayerState::Walking);
                }
                break;
            case SDLK_d:
                moveRight = false;
                if (!moveLeft) {
                    setState(PlayerState::Idle);
                } else if (isRunning) {
                    setState(PlayerState::Running);
                } else {
                    setState(PlayerState::Walking);
                }
                break;
            case SDLK_LSHIFT:
                isRunning = false;
                if (moveLeft || moveRight) {
                    setState(PlayerState::Walking);
                } else {
                    setState(PlayerState::Idle);
                }
                break;
        }

        if (!moveLeft && !moveRight && currentState == PlayerState::Walking) {
            setState(PlayerState::Idle);
        }
    }
}

void Player::setState(PlayerState newState) {
    if (currentState == newState) return;  // Prevent redundant updates

    currentState = newState;
    switch (newState) {
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
    }
}
void Player::updateMovement(float deltaTime) {
    isMoving = moveLeft || moveRight;
    float currentSpeed = isRunning ? runSpeed : walkSpeed;

    if (isMoving) {
        if (moveLeft) {
            getPos().x -= currentSpeed;
        }
        if (moveRight) {
            getPos().x += currentSpeed;
        }
    }
}

void Player::updateAnimation(float deltaTime) {
    Entity::updateAnimation(deltaTime);

    // If Jump or Attack finishes, return to Idle
    if ((currentState == PlayerState::Jumping || currentState == PlayerState::Attacking) && animationFinished()) {
        setState(PlayerState::Idle);
    }
}

