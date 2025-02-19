#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "include/RenderWindow.hpp"
#include "include/Entity.hpp"
#include "include/Utils.hpp"
#include "include/Player.hpp"
#include <math.h>
#include "include/npc.hpp"
#include <SDL_ttf.h>
#include "include/DialogueSystem.hpp"

// Create dialogue system instance
std::vector<NPC> npcs;

void setupNPCs(std::vector<NPC>& npcs, RenderWindow& window) {
    SDL_Texture* npcTexture = window.loadTexture("res/NPC/Idle.png");
   
    NPC npc1({150, 50}, npcTexture, 6, 128, 128);
    npc1.setDialogue({
        "Hello, traveler!",          
        "It's been a long time since someone visited.",
        "Do you need help?"
    });

    npcs.push_back(npc1);
}

Vector2f calculateDirection(const Vector2f& from, const Vector2f& to) {
    Vector2f direction = { to.x - from.x, to.y - from.y };
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction.x /= length;
        direction.y /= length;
    }
    return direction;
}

void handleEvents(bool& gameRunning, Player& player, DialogueSystem& dialogueSystem, std::vector<NPC>& npcs) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameRunning = false;
        }
        player.handleInput(event);
        dialogueSystem.handleInput(event);

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e) {
            for (NPC& npc : npcs) {
                if (npc.isPlayerNearby(player)) {
                    npc.interact(dialogueSystem);
                    break;
                }
            }
        }
    }
}

void updateNPCs(std::vector<NPC>& npcs, Player& player, SDL_Texture* npcLeftTexture, SDL_Texture* npcTexture, float frameTime) {
    for (NPC& npc : npcs) {
        if (npc.isPlayerNearby(player)) {
            Vector2f npcDirection = calculateDirection(npc.getPos(), player.getPos());
            SDL_Texture* newTexture = (npcDirection.x < 0) ? npcLeftTexture : npcTexture;

            if (npc.getTexture() != newTexture) {
                npc.setTexture(newTexture);
                npc.setFrameCount(6);
                npc.setFrameSize(128, 128);
                npc.setAnimationSpeed(0.05f);
                npc.resetAnimation();
            }
        } else {
            SDL_Texture* newTexture = npcTexture;
            if (npc.getTexture() != newTexture) {
                npc.setTexture(newTexture);
                npc.setFrameCount(6);
                npc.setFrameSize(128, 128);
                npc.setAnimationSpeed(0.05f);
                npc.resetAnimation();
            }
        }

        npc.updateAnimation(frameTime);
    }
}

void renderGame(RenderWindow& window, SDL_Texture* skyTexture, Player& player, std::vector<NPC>& npcs, DialogueSystem& dialogueSystem) {
    window.clear();
    window.renderSky(skyTexture);
    window.render(player);
    for (NPC& npc : npcs) {
        window.render(npc);
    }
    dialogueSystem.renderDialogue();
    window.display();
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "HEY... SDL_INIT HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "HEY... IMG_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }
    if (TTF_Init() == -1) {
        std::cout << "HEY... TTF_Init HAS FAILED. SDL_ERROR: " << TTF_GetError() << std::endl;
    }

    RenderWindow window("Creamy kayas Engine v0.1", 1280, 720);

    DialogueSystem dialogueSystem(window.getRenderer(), window.getFont());

    SDL_Texture *skyTexture = window.loadTexture("res/ComfyUI_00062_.png");
    SDL_Texture *idleTexture = window.loadTexture("res/player/Idle.png");
    SDL_Texture *walkTexture = window.loadTexture("res/player/Walk.png");
    SDL_Texture *jumpTexture = window.loadTexture("res/player/Jump.png");
    SDL_Texture *attack1Texture = window.loadTexture("res/player/Attack_1.png");
    SDL_Texture *runningTexture = window.loadTexture("res/player/Run.png");
    SDL_Texture *shieldTexture = window.loadTexture("res/player/Shield.png");

    SDL_Texture* npcLeftTexture = window.loadTexture("res/NPC/leftIdle.png");
    SDL_Texture *npcTexture = window.loadTexture("res/NPC/Idle.png");

    Player player({100, 50}, idleTexture, walkTexture, jumpTexture, attack1Texture, runningTexture, shieldTexture, 6, 128, 128);
    player.setAnimationSpeed(0.08f);

    bool gameRunning = true;
    const float timeStep = 0.01f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();

    setupNPCs(npcs, window);

    while (gameRunning) {
        float newTime = utils::hireTimeInSeconds();
        float frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        handleEvents(gameRunning, player, dialogueSystem, npcs);

        while (accumulator >= timeStep) {
            player.updateMovement(timeStep);
            player.updateAnimation(timeStep);
            updateNPCs(npcs, player, npcLeftTexture, npcTexture, timeStep);
            accumulator -= timeStep;
        }

        renderGame(window, skyTexture, player, npcs, dialogueSystem);
    }

    SDL_DestroyTexture(skyTexture);
    SDL_DestroyTexture(idleTexture);
    SDL_DestroyTexture(walkTexture);
    SDL_DestroyTexture(jumpTexture);
    SDL_DestroyTexture(attack1Texture);
    SDL_DestroyTexture(runningTexture);
    SDL_DestroyTexture(shieldTexture);
    SDL_DestroyTexture(npcTexture);

    TTF_CloseFont(window.getFont());
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}