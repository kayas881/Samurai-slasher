#include "../include/EventManager.hpp"

void EventManager::processEvents(bool& gameRunning, Player& player, DialogueSystem& dialogueSystem, std::vector<NPC>& npcs) {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameRunning = false;
        }
        
        player.handleInput(event);
        dialogueSystem.handleInput(event);

        // NPC interaction when pressing 'E'
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
