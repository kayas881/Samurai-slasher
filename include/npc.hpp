#pragma once
#include "Entity.hpp"
#include "DialogueSystem.hpp"
#include <vector>
#include <string>
class Player;
class NPC : public Entity {
public:
    NPC(Vector2f p_pos, SDL_Texture* p_texture, int p_frameCount, int p_frameWidth, int p_frameHeight);
    void setDialogue(std::vector<std::string> dialogueLines);
    void interact(DialogueSystem& dialogueSystem);
    bool isPlayerNearby(const Player& player);  
    SDL_Texture* getTexture() const;  // ✅ Add this function


private:
    std::vector<std::string> dialogues;
};