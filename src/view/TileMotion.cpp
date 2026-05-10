#include "view/TileMotion.hpp"
#include <algorithm>
#include <cmath>

TileMotion::TileMotion(float speedTilesPerSecond)
    : startTile{0, 0},
      endTile{0, 0},
      elapsed(0.0f),
      duration(0.0f),
      tilesPerSecond(std::max(1.0f, speedTilesPerSecond)),
      active(false) {}

void TileMotion::start(Point from, Point to) {
    startTile = from;
    endTile = to;
    elapsed = 0.0f;

    const float dx = static_cast<float>(to.x - from.x);
    const float dy = static_cast<float>(to.y - from.y);
    const float distanceInTiles = std::sqrt(dx * dx + dy * dy);

    if (distanceInTiles <= 0.001f) {
        duration = 0.0f;
        active = false;
        return;
    }

    duration = std::max(0.10f, distanceInTiles / tilesPerSecond);
    active = true;
}

void TileMotion::update(float dt) {
    if (!active) {
        return;
    }

    elapsed += std::max(0.0f, dt);
    if (elapsed >= duration) {
        elapsed = duration;
        active = false;
    }
}

void TileMotion::reset(Point position) {
    startTile = position;
    endTile = position;
    elapsed = 0.0f;
    duration = 0.0f;
    active = false;
}

bool TileMotion::isActive() const {
    return active;
}

Point TileMotion::getEndTile() const {
    return endTile;
}

Vector2 TileMotion::getCurrentTilePosition() const {
    if (!active || duration <= 0.0f) {
        return Vector2{static_cast<float>(endTile.x), static_cast<float>(endTile.y)};
    }

    const float t = std::min(1.0f, elapsed / duration);
    const float row = static_cast<float>(startTile.x) + static_cast<float>(endTile.x - startTile.x) * t;
    const float col = static_cast<float>(startTile.y) + static_cast<float>(endTile.y - startTile.y) * t;
    return Vector2{row, col};
}
