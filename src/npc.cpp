#include "../include/npc.hpp"
#include "../include/Player.hpp"

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
        dialogueSystem.startDialogue("Old Man", dialogues);
        dialogueSystem.resetDialogue();  // ✅ Reset if NPC is interacted with again
    }
}

SDL_Texture* NPC::getTexture() const {
    return text;  // ✅ Return the current texture
}

void NPC::setDirection(const Vector2f& newDirection) {
    direction = newDirection;
}

void NPC::updateBehavior(const Player& player, float deltaTime) {
    if (isPlayerNearby(player)) {
        setDirection(utills::calculateDirection(getPos(), player.getPos()));
        if (player.isPlayerAttacking()) {
            // React to player's attack
            std::cout << "NPC: Hey, watch it!" << std::endl;
        }
    }
}

void NPC::render(RenderWindow& window, bool isPlayerNearby) {
    SDL_RendererFlip flip = (isPlayerNearby && direction.x < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_Rect src = getCurrentFrame();
    SDL_Rect dst = {
        static_cast<int>(getPos().x * 4),
        static_cast<int>(getPos().y * 4),
        static_cast<int>(getCurrentFrame().w * 4),
        static_cast<int>(getCurrentFrame().h * 4)
    };

    SDL_RenderCopyEx(window.getRenderer(), getText(), &src, &dst, 0, NULL, flip);
}