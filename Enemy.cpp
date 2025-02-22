#include "include/Enemy.hpp"
#include "include/TextureManager.hpp"
#include <fstream>
#include <iostream>
#include "include/math.hpp"
#include "include/utils.hpp"
Enemy::Enemy(Vector2f p_pos, SDL_Renderer* renderer, const std::string& configPath)
    : Entity(p_pos, nullptr, 1, 128, 128) {
    
    // Load JSON configuration
    std::ifstream file(configPath);
    nlohmann::json config;
    file >> config;

    // Load enemy textures
    enemyIdleTexture = TextureManager::loadTexture(config["textures"]["idle"], renderer);
    enemyWalkTexture = TextureManager::loadTexture(config["textures"]["walk"], renderer);
    enemyAttackTexture = TextureManager::loadTexture(config["textures"]["attack"], renderer);
    enemyHurtTexture = TextureManager::loadTexture(config["textures"]["hurt"], renderer);
    enemyDeadTexture = TextureManager::loadTexture(config["textures"]["dead"], renderer);

    // Load stats
    hp = config["stats"]["hp"];
    attackPower = config["stats"]["attack_power"];
    defense = config["stats"]["defense"];
    speed = config["stats"]["speed"];

    setTexture(enemyIdleTexture);  // Default state
    setFrameCount(6);
}

// Enhanced AI movement and behavior
void Enemy::updateAI(float deltaTime, float distanceToEnemy, Vector2f enemyDirection) {
    if (isDead) return;  // ❌ Don't move if dead
    if(isEnemyHurt) return; // ❌ Don't move if hurt

    if (attackCooldown > 0.0f) {
        attackCooldown -= deltaTime;
    }
    // Determine state
    if (distanceToEnemy <= attackRange ) {
        if (currentState != EnemyState::Attak && attackCooldown <= 0.0f ) {
            setState(EnemyState::Attak);
            attackCooldown = attackCooldownDuration;
        }
    } else if (distanceToEnemy > stopChaseDistance) {
        if (currentState != EnemyState::Idle) {
            setState(EnemyState::Idle);
        }
    } else {
        if (currentState != EnemyState::Walk && currentState != EnemyState::Attak) {
            setState(EnemyState::Walk);
        }
    }

    // Move only if walking
    if (currentState == EnemyState::Walk) {
        pos.x += enemyDirection.x * speed * deltaTime;
        pos.y += enemyDirection.y * speed * deltaTime;
    }
}

// void Enemy::updateMovement(float deltaTime) {
//     // ✅ Implement movement logic
//     if(isDead) return;
// }
void Enemy::setState(EnemyState newState) {
    if (isDead) return;  // ❌ Ignore state changes if dead
    if (currentState == newState) return; // Prevent unnecessary updates

    currentState = newState;
    switch (newState)
    {
    case EnemyState::Idle:
        setTexture(enemyIdleTexture);
        setFrameCount(6);
        resetAnimation();  // ✅ Reset animation when switching
        break;
    case EnemyState::Walk:
        setTexture(enemyWalkTexture);
        setFrameCount(8);
        resetAnimation();  // ✅ Reset animation when switching
        break;
    case EnemyState::Attak:
        setTexture(enemyAttackTexture);
        setFrameCount(4);
        resetAnimation();
        break;
    case EnemyState::Hurt:
       
        setTexture(enemyHurtTexture);
        setFrameCount(3);
        resetAnimation();
        break;
    case EnemyState::Dead:
        setTexture(enemyDeadTexture);
        setFrameCount(3);
        isDead = true;
        break;
    }
}


// ✅ Damage system
void Enemy::takeDamage(Player &player, float distanceToEnemy, float deltaTime) {
    if (isDead) return; 

    // ✅ Check if the enemy is in range and was hit
    if (player.isPlayerAttacking() && distanceToEnemy <= attackRange && checkCollision(player.getBoundingBox(), getBoundingBox())) {
        if (!hasTakenDamage) {  // ✅ Only take damage once per attack
            setState(EnemyState::Hurt);
            hp -= std::max(0, player.getAttackPower() - defense);  // ✅ Prevent negative damage
            std::cout << "Enemy HP: " << hp << std::endl;

            isEnemyHurt = true;
            enemyHurtTime = 0.0f;  // ✅ Reset hurt timer
            hasTakenDamage = true;  // ✅ Mark this attack as applied

            if (hp <= 0) {
                setState(EnemyState::Dead);
                std::cout << "Enemy defeated!" << std::endl;
            }
        }
    }

    // ✅ Ensure the enemy stays hurt for a duration
    if (isEnemyHurt) {
        enemyHurtTime += deltaTime;
        if (enemyHurtTime >= enemyHurtDuration) {  // ✅ Wait for the full hurt duration
            isEnemyHurt = false;
            setState(EnemyState::Idle);  // ✅ Only switch to Idle after hurt animation is done
        }
    }

    // ✅ Reset damage flag when the player stops attacking
    if (!player.isPlayerAttacking()) {
        hasTakenDamage = false;
    }
}



void Enemy::updateAnimation(float deltaTime, float distanceToEnemy) {
    if (isDead && animationFinished()) return;

    Entity::updateAnimation(deltaTime);

    if (isDead) return;

    // ✅ If attack animation finished, return to idle (respect cooldown)
    if (currentState == EnemyState::Attak && animationFinished()) {
        setState(EnemyState::Idle);
    }

    // ✅ If hurt animation finished, return to previous state
    if (currentState == EnemyState::Hurt && animationFinished()) {
        setState(EnemyState::Idle);
    }

    // ✅ If the player is far away, return to idle
    if (currentState == EnemyState::Walk && distanceToEnemy > stopChaseDistance) {
        setState(EnemyState::Idle);
    }
}


