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

    loadTextures(renderer, config);

    // Load stats
    stats.hp = config["stats"]["hp"];
    stats.attackPower = config["stats"]["attack_power"];
    stats.defense = config["stats"]["defense"];
    stats.speed = config["stats"]["speed"];
    

    setTexture(textures[EnemyState::Idle]);  // Default state
    setFrameCount(6);
}

void Enemy::loadTextures(SDL_Renderer* renderer, const nlohmann::json& config) {
    textures[EnemyState::Idle] = TextureManager::loadTexture(config["textures"]["idle"], renderer);
    textures[EnemyState::Walk] = TextureManager::loadTexture(config["textures"]["walk"], renderer);
    textures[EnemyState::Attak] = TextureManager::loadTexture(config["textures"]["attack"], renderer);
    textures[EnemyState::Hurt] = TextureManager::loadTexture(config["textures"]["hurt"], renderer);
    textures[EnemyState::Dead] = TextureManager::loadTexture(config["textures"]["dead"], renderer);
}

// Enhanced AI movement and behavior
void Enemy::updateAI(float deltaTime, float distanceToEnemy, Vector2f enemyDirection, const Player& player) {
    if (isDead) return;  // ❌ Don't move if dead
    Enemydirection = enemyDirection;  // ✅ Store the direction before moving

    if (isEnemyHurt) return; // ❌ Don't move if hurt

    if (attackCooldown > 0.0f) {
        attackCooldown -= deltaTime;
    }

    handleStateTransitions(distanceToEnemy);
    handleMovement(deltaTime, distanceToEnemy);
}

void Enemy::handleStateTransitions(float distanceToEnemy) {
    if (distanceToEnemy <= attackRange) {
        if (currentState != EnemyState::Attak && attackCooldown <= 0.0f) {
            isMoving = false;  // Stop moving before attacking
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
            isMoving = true;  // Ensure it keeps moving
        }
    }
}


void Enemy::handleMovement(float deltaTime, float distanceToEnemy) {
    if (currentState == EnemyState::Walk && distanceToEnemy > attackRange) {
        if (!isMoving) isMoving = true; // Keep moving instead of toggling every frame
        pos.x += Enemydirection.x * stats.speed * deltaTime;
        pos.y += Enemydirection.y * stats.speed * deltaTime;
    } else if (currentState == EnemyState::Attak) {
        isMoving = false; // Stop movement only when attacking
    }

    std::cout<<"isMoving: "<<isMoving<<std::endl;
}


void Enemy::setState(EnemyState newState) {
    if (isDead) return;  // ❌ Ignore state changes if dead
    if (currentState == newState) return; // Prevent unnecessary updates

    currentState = newState;
    setTexture(textures[newState]);  // Set texture based on the new state
    resetAnimation();  // Reset animation when switching

    switch (newState)
    {
    case EnemyState::Idle:
        setFrameCount(6);
        break;
    case EnemyState::Walk:
        setFrameCount(8);
        break;
    case EnemyState::Attak:
        setFrameCount(4);
        break;
    case EnemyState::Hurt:
        setFrameCount(3);
        break;
    case EnemyState::Dead:
        setFrameCount(3);
        isDead = true;
        break;
    }
}

bool Enemy::isPlayerNearby(const Player& player) {
    SDL_Rect enemyBox = getBoundingBox();
    SDL_Rect playerBox = player.getBoundingBox();
    int distanceX = abs(playerBox.x - enemyBox.x);
    int distanceY = abs(playerBox.y - enemyBox.y);
    return (distanceX < 50 && distanceY < 50);
}
void Enemy::setDirection(const Vector2f& newDirection) {
    Enemydirection = newDirection;
}
// ✅ Damage system
void Enemy::takeDamage(Player &player, float distanceToEnemy, float deltaTime) {
    handleDamage(player, distanceToEnemy, deltaTime);
}

void Enemy::handleDamage(Player &player, float distanceToEnemy, float deltaTime) {
    if (isDead) return; 

    // ✅ Check if the enemy is in range and was hit
    if (player.isPlayerAttacking() && distanceToEnemy <= attackRange && checkCollision(player.getBoundingBox(), getBoundingBox())) {
        if (!hasTakenDamage) {  // ✅ Only take damage once per attack
            setState(EnemyState::Hurt);
            stats.hp -= std::max(0, player.getAttackPower() - stats.defense);  // ✅ Prevent negative damage
            std::cout << "Enemy HP: " << stats.hp << std::endl;

            isEnemyHurt = true;
            enemyHurtTime = 0.0f;  // ✅ Reset hurt timer
            hasTakenDamage = true;  // ✅ Mark this attack as applied

            if (stats.hp <= 0) {
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
    handleAnimation(deltaTime, distanceToEnemy);
}

void Enemy::handleAnimation(float deltaTime, float distanceToEnemy) {
    if (isDead && animationFinished()) return;
    Entity::updateAnimation(deltaTime);
    if (isDead) return;

    switch (currentState) {
        case EnemyState::Attak:
            if (animationFinished()) setState(EnemyState::Idle);
            break;
        case EnemyState::Walk:
            if (distanceToEnemy <= attackRange && attackCooldown <= 0.0f) {
                setState(EnemyState::Attak);
                attackCooldown = attackCooldownDuration;
            } else if (distanceToEnemy > stopChaseDistance) {
                setState(EnemyState::Idle);
            }
            break;
        case EnemyState::Hurt:
            if (animationFinished()) setState(EnemyState::Idle);
            break;
        default:
            break;
    }
}

void Enemy::render(RenderWindow& window, bool isPlayerNearby) {
    SDL_RendererFlip flip = (Enemydirection.x < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    
    SDL_Rect src = getCurrentFrame();
    SDL_Rect dst = {
        static_cast<int>(getPos().x * 4),
        static_cast<int>(getPos().y * 4),
        static_cast<int>(getCurrentFrame().w * 4),
        static_cast<int>(getCurrentFrame().h * 4)
    };

    SDL_RenderCopyEx(window.getRenderer(), getText(), &src, &dst, 0, NULL, flip);
}
