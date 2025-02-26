#pragma once
#include "Entity.hpp"
#include <SDL.h>
#include <string>
#include <vector>
#include <json.hpp>
#include "Player.hpp"
#include "renderWindow.hpp"
#include "math.hpp"
enum class EnemyState{Idle, Walk, Attak, Hurt, Dead };
class Player;
class Enemy : public Entity {
public:
    Enemy(Vector2f p_pos, SDL_Renderer* renderer, const std::string& configPath);

    void updateAI(float deltaTime, float distanceToEnemy, Vector2f enemyDirection, const Player& Player); // AI behavior
    void takeDamage(Player &player,float distanceToEnemy,float deltaTime); // ✅ Damage system
    bool isDefeated() const { return hp <= 0; }
    void updateAnimation(float deltaTime,float distanceToEnemy);
    void updateMovement(float deltaTime);
    void setState(EnemyState newState);
    SDL_Texture* getTexture() const { return text; }
    bool isPlayerNearby(const Player& player);  
    void render(RenderWindow& window, bool isPlayerNearby);
    void setDirection(const Vector2f& newDirection);  // Add this setter method

    
    
private:
    SDL_Texture* enemyIdleTexture;
    SDL_Texture* enemyWalkTexture;
    SDL_Texture* enemyAttackTexture;
    SDL_Texture* enemyHurtTexture;
    SDL_Texture* enemyDeadTexture;
    
    bool isDead = false;
    int hp;
    int attackPower;
    int defense;
    float speed;
    float attackRange = 40.0f;  // You can tweak this value based on your game
    float stopChaseDistance = 80.0f; 
    bool hasTakenDamage = false; 
    float enemyHurtDuration = 0.25f;
    float enemyHurtTime = 0.0f;
    bool isEnemyHurt = false;
    float attackCooldown = 0.0f;  // ✅ Timer for cooldown
    float attackCooldownDuration = 2.0f;  // ✅ Enemy can attack every 2 seconds
    Vector2f Enemydirection;  // Store the direction of the NPC
    EnemyState currentState = EnemyState::Idle;
};
