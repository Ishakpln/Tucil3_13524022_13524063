#pragma once

#include "raylib.h"
#include "utils/Helper.hpp"

class TileMotion {
private:
    Point startTile;
    Point endTile;
    float elapsed;
    float duration;
    float tilesPerSecond;
    bool active;

public:
    explicit TileMotion(float speedTilesPerSecond = 8.0f);

    void start(Point from, Point to);
    void update(float dt);
    void reset(Point position);

    bool isActive() const;
    Point getEndTile() const;

    // Vector2.x stores the interpolated board row and Vector2.y stores the interpolated board column.
    Vector2 getCurrentTilePosition() const;

    // Player art faces upward by default. This returns the rotation needed while sliding.
    // When the player is not sliding, it returns 0 so the player faces upward again.
    float getRotationFromUpDegrees() const;
};
