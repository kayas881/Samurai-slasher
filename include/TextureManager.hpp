#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <string>

class TextureManager {
public:
    static SDL_Texture* loadTexture(const std::string& filePath, SDL_Renderer* renderer);
    static void cleanup();  // Frees all textures
private:
    static std::unordered_map<std::string, SDL_Texture*> textures;
};
