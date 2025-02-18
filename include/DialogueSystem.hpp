#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

class DialogueSystem {
public:
    DialogueSystem(SDL_Renderer* renderer, TTF_Font* font);
    void startDialogue(const std::string& npcName, const std::vector<std::string>& lines);
    void renderDialogue();
    void handleInput(SDL_Event& event);
    bool isActive() const;
    void resetDialogue();

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::string npcName;
    std::vector<std::string> dialogueLines;
    int currentLine;
    bool active;
    bool dialogueFinished;  // ✅ Track if dialogue has finished

};
