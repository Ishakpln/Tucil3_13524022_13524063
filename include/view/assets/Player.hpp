#pragma once

#include "raylib.h"
#include "view/assets/TextureResource.hpp"
#include <array>
#include <memory>
#include <string>
#include <vector>

class Player {
protected:
    std::string playerType;
    std::vector<TextureResource> frames;
    std::array<int, 4> animationOrder{{0, 1, 0, 2}};
    float frameDuration = 0.15f;
    float elapsed = 0.0f;
    int sequenceIndex = 0;

public:
    explicit Player(std::string typeName);
    virtual ~Player() = default;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    const std::string& getType() const;
    virtual bool usesContinuousAnimation() const;
    virtual bool usesMovementRotation() const;
    void update(float dt, bool moving = true);
    void resetAnimation();
    void drawAt(float boardX, float boardY, float tileSize, int row, int col, float rotationDegrees = 0.0f) const;
    void drawAtTilePosition(float boardX, float boardY, float tileSize, float row, float col, float rotationDegrees = 0.0f) const;
};

class Baby : public Player {
public:
    Baby();
};

class Fire : public Player {
public:
    Fire();
    bool usesContinuousAnimation() const override;
    bool usesMovementRotation() const override;
};

std::unique_ptr<Player> createPlayerByType(const std::string& playerType);
