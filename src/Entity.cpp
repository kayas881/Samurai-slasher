#include "../include/Entity.hpp"

Entity::Entity(Vector2f p_pos, SDL_Texture* p_text, int p_frameCount, int p_frameWidth, int p_frameHeight)
    : pos(p_pos), text(p_text), frameCount(p_frameCount), frameWidth(p_frameWidth), frameHeight(p_frameHeight), 
    animationSpeed(0.1f), timeSinceLastFrame(0), currentFrameIndex(0), animationRow(0)
    {
        currentFrame = {0, 0, frameWidth, frameHeight};
    }


void Entity::updateAnimation(float deltaTime){
    timeSinceLastFrame += deltaTime;
    if(timeSinceLastFrame >= animationSpeed){
        timeSinceLastFrame = 0;
        currentFrameIndex = (currentFrameIndex + 1) % frameCount;
        currentFrame.x = currentFrameIndex * frameWidth;  // Move horizontally across the sprite sheet
        currentFrame.y = animationRow * frameHeight;       // Stay in the same row
        timeSinceLastFrame = 0;
    }
}



bool Entity::animationFinished() const {
    return currentFrameIndex == (frameCount - 1);
}

void Entity::resetAnimation() {
    currentFrameIndex = 0;
    currentFrame.x = 0;
    timeSinceLastFrame = 0;
}

void Entity::setFrameSize(int width, int height) {
    frameWidth = width;
    frameHeight = height;
    currentFrame.w = frameWidth;
    currentFrame.h = frameHeight;
}

void Entity::setAnimationRow(int row) {
    animationRow = row;
}

void Entity::setTexture(SDL_Texture* newTexture) {
    text = newTexture;
    resetAnimation();  // Reset animation when switching textures
}

SDL_Rect Entity::getBoundingBox() const {
    return { static_cast<int>(pos.x), static_cast<int>(pos.y), frameWidth, frameHeight };
}

void Entity::setFrameCount(int count) {
    frameCount = count;
}

bool Entity::checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b);
}