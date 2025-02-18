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

int main(int argc, char* argv[])
{
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

    SDL_Texture *npcTexture = window.loadTexture("res/NPC/Idle.png");


    Player player({100, 50}, idleTexture, walkTexture, jumpTexture, attack1Texture, runningTexture,shieldTexture, 6, 128, 128);
    player.setAnimationSpeed(0.05f);

    bool gameRunning = true;
    SDL_Event event;

    const float timeStep = 0.01f;
    float accumulator = 0.01f;
    float currentTime = utils::hireTimeInSeconds();
    bool showDialogue = false;
    std::string dialogueText = "";
    
    
    setupNPCs(npcs, window);
    while(gameRunning){
        float newTime = utils::hireTimeInSeconds();
        float frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while(accumulator >= timeStep){
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    gameRunning = false;
                }
                player.handleInput(event);  // ✅ Player now handles its own input
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

            player.updateMovement(frameTime);  // ✅ Player moves itself
            player.updateAnimation(frameTime);  // ✅ Player updates its own animation
            for (NPC& npc : npcs) {
                npc.updateAnimation(frameTime);
            }
            accumulator -= timeStep;
        }

        window.clear();
        window.renderSky(skyTexture);
        window.render(player);
        for (NPC& npc : npcs) {
            window.render(npc);
        }
        dialogueSystem.renderDialogue();  // ✅ Now dialogue is displayed in the game

        window.display();
    }

    SDL_DestroyTexture(skyTexture);
    SDL_DestroyTexture(idleTexture);
    SDL_DestroyTexture(attack1Texture);
    SDL_Quit();
    IMG_Quit();

    return 0;
}
