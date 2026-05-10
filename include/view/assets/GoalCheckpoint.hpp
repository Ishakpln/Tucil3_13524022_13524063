#pragma once

#include "raylib.h"
#include "view/assets/TextureResource.hpp"
#include <string>

class GoalCheckpoint {
private:
    std::string playerType;
    TextureResource checkpointTexture;
    TextureResource goalTexture;

public:
    explicit GoalCheckpoint(const std::string& playerType = "Baby");

    void reloadForPlayer(const std::string& newPlayerType);
    void drawCheckpoint(float boardX, float boardY, float tileSize, int row, int col, char label) const;
    void drawGoal(float boardX, float boardY, float tileSize, int row, int col) const;
};
