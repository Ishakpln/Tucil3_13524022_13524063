#pragma once

#include "raylib.h"
#include "view/assets/TextureResource.hpp"
#include <string>

// Base class for all obstacle-like GUI assets. It does not affect Board logic.
class Obstacle {
protected:
    TextureResource texture;
    std::string assetName;
    int widthInTiles = 1;
    int heightInTiles = 1;

public:
    Obstacle(std::string name, const std::string& texturePath);
    virtual ~Obstacle() = default;

    Obstacle(const Obstacle&) = delete;
    Obstacle& operator=(const Obstacle&) = delete;
    Obstacle(Obstacle&&) noexcept = default;
    Obstacle& operator=(Obstacle&&) noexcept = default;

    const std::string& name() const;
    bool isLoaded() const;
    int getWidthInTiles() const;
    int getHeightInTiles() const;

    virtual float getRotationFacingCenter(int row, int col, int boardRows, int boardCols) const;

    void drawAt(float boardX, float boardY, float tileSize, int row, int col) const;
    void drawAt(float boardX, float boardY, float tileSize, int row, int col,
                int drawWidthTiles, int drawHeightTiles, float rotationDegrees = 0.0f) const;
};
