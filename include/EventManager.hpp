#pragma once
#include <SDL.h>
#include "Player.hpp"
#include "DialogueSystem.hpp"
#include "npc.hpp"

class EventManager {
public:
    static void processEvents(bool& gameRunning, Player& player, DialogueSystem& dialogueSystem, std::vector<NPC>& npcs);
};
