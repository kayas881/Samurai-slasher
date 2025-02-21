#include "include/Enemy.hpp"
#include "include/TextureManager.hpp"
#include <fstream>
#include <iostream>

Enemy::Enemy(Vector2f p_pos, SDL_Renderer* renderer, const std::string& configPath)
    : Entity(p_pos, nullptr, 1, 128, 128) {
    
    // Load JSON configuration
    std::ifstream file(configPath);
    nlohmann::json config;
    file >> config;

    // Load enemy textures
    idleTexture = TextureManager::loadTexture(config["textures"]["idle"], renderer);
    walkTexture = TextureManager::loadTexture(config["textures"]["walk"], renderer);
    attackTexture = TextureManager::loadTexture(config["textures"]["attack"], renderer);
    hurtTexture = TextureManager::loadTexture(config["textures"]["hurt"], renderer);
    deadTexture = TextureManager::loadTexture(config["textures"]["dead"], renderer);

    // Load stats
    hp = config["stats"]["hp"];
    attackPower = config["stats"]["attack_power"];
    defense = config["stats"]["defense"];
    speed = config["stats"]["speed"];

    setTexture(idleTexture);  // Default state
    setFrameCount(6);
}

// Enhanced AI movement and behavior
void Enemy::updateAI(float deltaTime, Vector2f playerPos) {
    if (hp <= 0) return;  // ❌ Stop movement if dead

    float distanceToPlayer = std::abs(playerPos.x - getPos().x);

    if (distanceToPlayer < attackRange) {
        // Attack player
        if (getTexture() != attackTexture) {
            setTexture(attackTexture);
            setFrameCount(6);
            resetAnimation();
        }
    } else if (distanceToPlayer < detectionRange) {
        // Move towards player
        if (playerPos.x < getPos().x) {
            getPos().x -= speed * deltaTime;  // Move left
        } else if (playerPos.x > getPos().x) {
            getPos().x += speed * deltaTime;  // Move right
        }
        if (getTexture() != walkTexture) {
            setTexture(walkTexture);
            setFrameCount(8);
            resetAnimation();
        }
    } else {
        // Idle state
        if (getTexture() != idleTexture) {
            setTexture(idleTexture);
            setFrameCount(6);
        }
    }

    Entity::updateAnimation(deltaTime);
}

// ✅ Damage system
void Enemy::takeDamage(int damage) {
    hp -= std::max(0, damage - defense); // Reduce damage by defense
    std::cout << "Enemy HP: " << hp << std::endl;

    if (hp > 0) {
        setTexture(hurtTexture);
        setFrameCount(3);
    } else {
        setTexture(deadTexture);
        setFrameCount(3);
    }
}
