#include "../include/TextureManager.hpp"
#include <iostream>

// Initialize the static texture map
std::unordered_map<std::string, SDL_Texture*> TextureManager::textures;

SDL_Texture* TextureManager::loadTexture(const std::string& filePath, SDL_Renderer* renderer) {
    // Check if texture is already loaded
    if (textures.find(filePath) != textures.end()) {
        return textures[filePath];  // Return existing texture
    }

    // Load new texture
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
    if (!texture) {
        std::cout << "Failed to load texture: " << filePath << " ERROR: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // Store in map
    textures[filePath] = texture;
    return texture;
}

void TextureManager::cleanup() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
}
