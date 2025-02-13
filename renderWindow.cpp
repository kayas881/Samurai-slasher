#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include "include/RenderWindow.hpp"
#include "include/Entity.hpp"

RenderWindow::RenderWindow(const char *p_title, int p_w, int p_h)
    : window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDL_Texture *RenderWindow::loadTexture(const char *p_filePath)
{
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL)
    {
        std::cout << "Failed to load texture. ERROR: " << SDL_GetError() << std::endl;
    }

    return texture;
}

int RenderWindow::getRefreshRate()
{
    int displayIndex = SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex,0,&mode);
    return mode.refresh_rate;
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::renderSky(SDL_Texture *p_texture)

{
    SDL_Rect dst = {0, 0, 1280, 720};
    SDL_RenderCopy(renderer, p_texture, NULL, &dst);
}

void RenderWindow::renderPath(Entity &p_entity)
{
    SDL_Rect dst = {0, 0, 1280, 720};
    SDL_RenderCopy(renderer, p_entity.getText(), NULL, &dst);
}


void RenderWindow::render(Entity& p_entity) {
    SDL_Rect src = p_entity.getCurrentFrame();
    SDL_Rect dst = {
        static_cast<int>(p_entity.getPos().x * 4),
        static_cast<int>(p_entity.getPos().y * 4),
        static_cast<int>(p_entity.getCurrentFrame().w * 4),
        static_cast<int>(p_entity.getCurrentFrame().h * 4)
    };

    SDL_RenderCopy(renderer, p_entity.getText(), &src, &dst);
}

void RenderWindow::display() {
    SDL_RenderPresent(renderer);
}