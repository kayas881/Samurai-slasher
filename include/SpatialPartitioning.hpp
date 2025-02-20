#pragma once
#include <vector>
#include "npc.hpp"
#include "Player.hpp"
class SpatialPartitioning {
public:
    SpatialPartitioning(int width, int height, int cellSize);
    void addNPC(NPC* npc);
    void clear();
    std::vector<NPC*> getNearbyNPCs(const Player& player);

private:
    int width, height, cellSize;
    std::vector<std::vector<std::vector<NPC*>>> grid;
};
