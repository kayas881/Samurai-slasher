#pragma once
#include "Entity.hpp"
#include <SDL.h>
#include <string>
#include <vector>
#include <json.hpp>

class Enemy : public Entity {
public:
    Enemy(Vector2f p_pos, SDL_Renderer* renderer, const std::string& configPath);

    void updateAI(float deltaTime, Vector2f playerPos); // AI behavior
    void takeDamage(int damage);
    bool isDefeated() const { return hp <= 0; }

    SDL_Texture* getTexture() const { return text; }
private:
    SDL_Texture* idleTexture;
    SDL_Texture* walkTexture;
    SDL_Texture* attackTexture;
    SDL_Texture* hurtTexture;
    SDL_Texture* deadTexture;

    int hp;
    int attackPower;
    int defense;
    float speed;
    float attackRange = 50.0f;  // Range within which the enemy attacks the player
    float detectionRange = 200.0f;  // Range within which the enemy detects the player
};
