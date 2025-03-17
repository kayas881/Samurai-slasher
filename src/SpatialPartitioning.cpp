#include "../include/SpatialPartitioning.hpp"

SpatialPartitioning::SpatialPartitioning(int width, int height, int cellSize)
    : width(width), height(height), cellSize(cellSize) {
    int cols = width / cellSize;
    int rows = height / cellSize;
    grid.resize(cols, std::vector<std::vector<NPC*>>(rows));
}

void SpatialPartitioning::addNPC(NPC* npc) {
    int col = npc->getPos().x / cellSize;
    int row = npc->getPos().y / cellSize;
    grid[col][row].push_back(npc);
}

void SpatialPartitioning::clear() {
    for (auto& col : grid) {
        for (auto& cell : col) {
            cell.clear();
        }
    }
}

std::vector<NPC*> SpatialPartitioning::getNearbyNPCs(const Player& player) {
    std::vector<NPC*> nearbyNPCs;
    int col = player.getPos().x / cellSize;
    int row = player.getPos().y / cellSize;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newCol = col + i;
            int newRow = row + j;
            if (newCol >= 0 && newCol < grid.size() && newRow >= 0 && newRow < grid[0].size()) {
                for (NPC* npc : grid[newCol][newRow]) {
                    nearbyNPCs.push_back(npc);
                }
            }
        }
    }
    return nearbyNPCs;
}
