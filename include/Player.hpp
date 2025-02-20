#pragma once
#include "Entity.hpp"
#include <SDL.h>

enum class PlayerState { Idle, Walking, Jumping, Attacking, Running, Shielding, Hurt, Dead };

class Player : public Entity {
public:
    Player(Vector2f p_pos, SDL_Texture* idleTexture, SDL_Texture* walkTexture, SDL_Texture* jumpTexture, SDL_Texture* attackTexture,
        SDL_Texture* runningTexture, SDL_Texture* shieldTexture, SDL_Texture* hurtTexture,SDL_Texture* deadTexture,
           int frameCount = 1, int frameWidth = 128, int frameHeight = 128);

    void handleInput(const SDL_Event& event);
    void updateMovement(float deltaTime);
    void updateAnimation(float deltaTime);
    void setState(PlayerState newState);
    bool isPlayerDead();


private:
    SDL_Texture* idleTexture;
    SDL_Texture* walkTexture;
    SDL_Texture* jumpTexture;
    SDL_Texture* attackTexture;
    SDL_Texture* runningTexture;
    SDL_Texture* shieldTexture;
    SDL_Texture* hurtTexture;
    SDL_Texture* deadTexture;
    PlayerState currentState;
    bool isDead = false; // Prevents movement & state changes when dead
    float jumpVelocity;       // Vertical velocity during jump
    float gravity;            // Gravity force pulling the character down
    bool isJumping;           // Whether the character is currently jumping
    float groundLevel;        // The Y position where the character stands (ground level)
    float hp = 100;
};
