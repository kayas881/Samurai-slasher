#include "include/DialogueSystem.hpp"

DialogueSystem::DialogueSystem(SDL_Renderer* p_renderer, TTF_Font* p_font)
    : renderer(p_renderer), font(p_font), currentLine(0), active(false) {}

    void DialogueSystem::startDialogue(const std::string& name, const std::vector<std::string>& lines) {
        if (dialogueFinished) return;  // ✅ Prevent restarting if already finished
    
        npcName = name;
        dialogueLines = lines;
        currentLine = 0;
        active = true;
        dialogueFinished = false;  // ✅ Reset flag when starting new dialogue
    }
    
    void DialogueSystem::renderDialogue() {
        if (!active || dialogueLines.empty()) return;
    
        // Define box dimensions
        SDL_Rect boxRect = {230, 600, 800, 100};  // x, y, width, height
    
        // Set box color (black with transparency)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);  // RGBA (black with alpha)
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // Enable blending
        SDL_RenderFillRect(renderer, &boxRect);
    
        SDL_Color textColor = {255, 255, 255};  // White text
    
        // Render NPC Name (above dialogue box)
        SDL_Surface* nameSurface = TTF_RenderText_Solid(font, npcName.c_str(), {255, 255, 0});  // Yellow name
        SDL_Texture* nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
        SDL_Rect nameRect = {240, 570, nameSurface->w, nameSurface->h};  // Position above box
        SDL_RenderCopy(renderer, nameTexture, nullptr, &nameRect);
        SDL_FreeSurface(nameSurface);
        SDL_DestroyTexture(nameTexture);
    
        // Render dialogue text inside the box
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, dialogueLines[currentLine].c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {240, 620, textSurface->w, textSurface->h};  // Position inside box
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    void DialogueSystem::handleInput(SDL_Event& event) {
        if (!active) return;
    
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e) {
            currentLine++;
    
            if (currentLine >= dialogueLines.size()) {
                active = false;        // ✅ Hide dialogue when finished
                dialogueFinished = true; // ✅ Mark as finished
            }
        }
    }
    

bool DialogueSystem::isActive() const {
    return active;
}
void DialogueSystem::resetDialogue() {
    dialogueFinished = false;  // ✅ Allow dialogue to start again
}