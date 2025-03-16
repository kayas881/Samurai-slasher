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
#include "include/Enemy.hpp"

// Global variables
std::vector<NPC> npcs;
SpatialPartitioning spatialPartitioning(1280, 720, 200);  // Adjust cell size as needed

// Function to set up NPCs
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

// Function to update NPCs
void updateNPCs(SpatialPartitioning& spatialPartitioning, Player& player, float frameTime) {
    spatialPartitioning.clear();
    for (NPC& npc : npcs) {
        spatialPartitioning.addNPC(&npc);
    }

    std::vector<NPC*> nearbyNPCs = spatialPartitioning.getNearbyNPCs(player);
    for (NPC* npc : nearbyNPCs) {
        npc->updateBehavior(player, frameTime);
        npc->updateAnimation(frameTime);
    }
}

// Function to render the game
void renderGame(RenderWindow& window, SDL_Texture* skyTexture, Player& player, Enemy& enemy, std::vector<NPC>& npcs, DialogueSystem& dialogueSystem) {
    window.clear();
    window.renderSky(skyTexture);

    for (NPC& npc : npcs) {
        bool nearby = npc.isPlayerNearby(player);
        npc.render(window, nearby);
        
    }
    
    window.render(player); 
    bool isPlayerNearby = enemy.isPlayerNearby(player);
    enemy.render(window, isPlayerNearby);
    dialogueSystem.renderDialogue();
    window.display();
}

// Main function
int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "HEY... SDL_INIT HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "HEY... IMG_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cout << "HEY... TTF_Init HAS FAILED. SDL_ERROR: " << TTF_GetError() << std::endl;
    }

    // Create window
    RenderWindow window("Creamyyy kayas Engine v0.1", 1280, 720);
    
    // Initialize dialogue system
    DialogueSystem dialogueSystem(window.getRenderer(), window.getFont());

    // Load textures
    SDL_Texture *skyTexture = TextureManager::loadTexture("res/ComfyUI_00062_.png", window.getRenderer());
    SDL_Texture *npcTexture = TextureManager::loadTexture("res/NPC/Idle.png", window.getRenderer());
 
    // Initialize player and enemy
    Player player({100, 50}, window.getRenderer(), "player.json");
    player.setAnimationSpeed(0.08f);

    Enemy enemy({200, 50}, window.getRenderer(), "Enemy.json");
    enemy.setAnimationSpeed(0.08f);

    // Game loop variables
    bool gameRunning = true;
    const float timeStep = 0.01f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();

    // Set up NPCs
    setupNPCs(npcs, window, spatialPartitioning);

    // Game loop
    while (gameRunning) {
        float newTime = utils::hireTimeInSeconds();
        float frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;
        // Process events
        EventManager::processEvents(gameRunning, player, dialogueSystem, npcs);
        // Update game state
        while (accumulator >= timeStep) {
            player.updateMovement(timeStep);
            player.updateAnimation(timeStep);
            Vector2f enemyDirection = utills::calculateDirection(enemy.getPos(), player.getPos());
            float distanceToEnemy = utills::calculateDistance(player.getPos(), enemy.getPos());
            enemy.updateAI(timeStep, distanceToEnemy, enemyDirection, player);
            enemy.updateAnimation(timeStep, distanceToEnemy);
            enemy.takeDamage(player, distanceToEnemy, timeStep);
            updateNPCs(spatialPartitioning, player, timeStep);
            accumulator -= timeStep;
        }

        // Render game
        renderGame(window, skyTexture, player, enemy, npcs, dialogueSystem);
    }

    // Cleanup
    TextureManager::cleanup();

    TTF_CloseFont(window.getFont());
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}