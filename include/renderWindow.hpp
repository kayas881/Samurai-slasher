#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Entity.hpp"
#include <SDL_ttf.h>
class RenderWindow
{
    public:
    RenderWindow(const char* p_title, int p_w, int p_h);
    SDL_Texture* loadTexture(const char* p_filePath);
    int getRefreshRate();
    void cleanUp();
    void clear();
    void  renderSky(SDL_Texture* p_texture);
    void renderPath(Entity& p_entity);
    void render(Entity& p_entity);
    void display();

    // Getters
    SDL_Renderer* getRenderer() { return renderer; }
    SDL_Window* getWindow() { return window; }
    TTF_Font* getFont() { return font; }
    

    private:
    SDL_Window* window;
    SDL_Renderer*  renderer;
    TTF_Font* font;


};