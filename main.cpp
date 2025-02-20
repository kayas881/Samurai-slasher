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
#include "include/TextureManager.hpp"
#include "include/EventManager.hpp"
#include "include/utils.hpp"
#include "include/SpatialPartitioning.hpp"
std::vector<NPC> npcs;

SpatialPartitioning spatialPartitioning(1280, 720, 200);  // Adjust cell size as needed

void setupNPCs(std::vector<NPC>& npcs, RenderWindow& window, SpatialPartitioning& spatialPartitioning) {
    SDL_Texture* npcTexture = TextureManager::loadTexture("res/NPC/Idle.png", window.getRenderer());
   
    NPC npc1({150, 50}, npcTexture, 6, 128, 128);
    npc1.setDialogue({
        "Hello, traveler!",          
        "It's been a long time since someone visited.",
        "Do you need help?"
    });

    npcs.push_back(npc1);
    spatialPartitioning.addNPC(&npc1);
}

void updateNPCs(SpatialPartitioning& spatialPartitioning, Player& player, SDL_Texture* npcLeftTexture, SDL_Texture* npcTexture, float frameTime) {
    spatialPartitioning.clear();
    for (NPC& npc : npcs) {
        spatialPartitioning.addNPC(&npc);
    }

    std::vector<NPC*> nearbyNPCs = spatialPartitioning.getNearbyNPCs(player);
    for (NPC* npc : nearbyNPCs) {
        if (npc->isPlayerNearby(player)) {
            Vector2f npcDirection = utills::calculateDirection(npc->getPos(), player.getPos());
            SDL_Texture* newTexture = (npcDirection.x < 0) ? npcLeftTexture : npcTexture;

            if (npc->getTexture() != newTexture) {
                npc->setTexture(newTexture);
                npc->setFrameCount(6);
                npc->setFrameSize(128, 128);
                npc->setAnimationSpeed(0.05f);
                npc->resetAnimation();
            }
        } else {
            SDL_Texture* newTexture = npcTexture;
            if (npc->getTexture() != newTexture) {
                npc->setTexture(newTexture);
                npc->setFrameCount(6);
                npc->setFrameSize(128, 128);
                npc->setAnimationSpeed(0.05f);
                npc->resetAnimation();
            }
        }

        npc->updateAnimation(frameTime);
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

    SDL_Texture *skyTexture = TextureManager::loadTexture("res/ComfyUI_00062_.png", window.getRenderer());
    SDL_Texture *idleTexture = TextureManager::loadTexture("res/player/Idle.png", window.getRenderer());

    SDL_Texture *walkTexture = TextureManager::loadTexture("res/player/Walk.png", window.getRenderer());
    SDL_Texture *jumpTexture = TextureManager::loadTexture("res/player/Jump.png", window.getRenderer());
    SDL_Texture *attack1Texture = TextureManager::loadTexture("res/player/Attack_1.png", window.getRenderer());
    SDL_Texture *runningTexture = TextureManager::loadTexture("res/player/Run.png", window.getRenderer());
    SDL_Texture *shieldTexture = TextureManager::loadTexture("res/player/Shield.png", window.getRenderer());
    SDL_Texture *hurtTexture = TextureManager::loadTexture("res/player/Hurt.png", window.getRenderer());
    SDL_Texture* npcLeftTexture = TextureManager::loadTexture("res/NPC/leftIdle.png", window.getRenderer());
    SDL_Texture *npcTexture = TextureManager::loadTexture("res/NPC/Idle.png", window.getRenderer());
    SDL_Texture *deadTexture = TextureManager::loadTexture("res/player/Dead.png", window.getRenderer());
 
    Player player({100, 50}, idleTexture, walkTexture, jumpTexture, attack1Texture, runningTexture, shieldTexture,hurtTexture,deadTexture, 6, 128, 128);
    player.setAnimationSpeed(0.08f);

    bool gameRunning = true;
    const float timeStep = 0.01f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();

    setupNPCs(npcs, window, spatialPartitioning);

    while (gameRunning) {
        float newTime = utils::hireTimeInSeconds();
        float frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;
        EventManager::processEvents(gameRunning, player, dialogueSystem, npcs);
        while (accumulator >= timeStep) {
            player.updateMovement(timeStep);
            player.updateAnimation(timeStep);
            updateNPCs(spatialPartitioning, player, npcLeftTexture, npcTexture, timeStep);
            accumulator -= timeStep;
        }

        renderGame(window, skyTexture, player, npcs, dialogueSystem);
    }

    TextureManager::cleanup();

    TTF_CloseFont(window.getFont());
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}