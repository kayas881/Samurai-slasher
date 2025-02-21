#pragma once
#include "Entity.hpp"
#include <SDL.h>
#include "json.hpp"
#include "Enemy.hpp"
enum class PlayerState { Idle, Walking, Jumping, Attacking, Running, Shielding, Hurt, Dead };

class Player : public Entity {
public:
    Player(Vector2f p_pos, SDL_Renderer* renderer, const std::string& configPath);

    void handleInput(const SDL_Event& event);
    void updateMovement(float deltaTime);
    void updateAnimation(float deltaTime);
    void setState(PlayerState newState);

    int getWidth() const;  // New method to get player width
    int getHeight() const; // New method to get player height

private:
    bool checkCollisionWithEnemy();
    void takeDamage(int damage);

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
    int attackPower;          // New member variable for attack power
};