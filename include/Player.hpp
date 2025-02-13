#pragma once
#include "Entity.hpp"
#include <SDL.h>

enum class PlayerState { Idle, Walking, Jumping, Attacking, Running, Shielding };

class Player : public Entity {
public:
    Player(Vector2f p_pos, SDL_Texture* idleTexture, SDL_Texture* walkTexture, SDL_Texture* jumpTexture, SDL_Texture* attackTexture,
        SDL_Texture* runningTexture, SDL_Texture* shieldTexture,
           int frameCount = 1, int frameWidth = 128, int frameHeight = 128);

    void handleInput(const SDL_Event& event);
    void updateMovement(float deltaTime);
    void updateAnimation(float deltaTime);
    void setState(PlayerState newState);

private:
    SDL_Texture* idleTexture;
    SDL_Texture* walkTexture;
    SDL_Texture* jumpTexture;
    SDL_Texture* attackTexture;
    SDL_Texture* runningTexture;
    SDL_Texture* shieldTexture;
    PlayerState currentState;
};
