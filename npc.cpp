#include "include/npc.hpp"
#include "include/Player.hpp"


NPC::NPC(Vector2f p_pos, SDL_Texture* p_texture, int p_frameCount, int p_frameWidth, int p_frameHeight)
    : Entity(p_pos, p_texture, p_frameCount, p_frameWidth, p_frameHeight) {}

void NPC::setDialogue(std::vector<std::string> dialogueLines) {
    dialogues = dialogueLines;
}

bool NPC::isPlayerNearby(const Player& player) {
    SDL_Rect npcBox = getBoundingBox();
    SDL_Rect playerBox = player.getBoundingBox();
    int distanceX = abs(playerBox.x - npcBox.x);
    int distanceY = abs(playerBox.y - npcBox.y);
    return (distanceX < 50 && distanceY < 50);
}

void NPC::interact(DialogueSystem& dialogueSystem) {
    if (!dialogueSystem.isActive()) {
        dialogueSystem.resetDialogue();  // ✅ Reset if NPC is interacted with again
        dialogueSystem.startDialogue("Old Man", dialogues);
    }
}

