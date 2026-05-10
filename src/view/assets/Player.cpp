#include "view/assets/Player.hpp"
#include "view/assets/AssetUtils.hpp"
#include <utility>

Player::Player(std::string typeName)
    : playerType(std::move(typeName)) {
    frames.reserve(3);

    for (int i = 1; i <= 3; ++i) {
        std::string path = "./assets/players/" + playerType + "/" + std::to_string(i) + ".png";
        if (!FileExists(path.c_str()) && i > 1) {
            // Some future/incomplete player folders may not contain all 3 images yet.
            // Reuse frame 1 so animation logic still stays 1-2-1-3 without crashing.
            path = "./assets/players/" + playerType + "/1.png";
        }
        frames.emplace_back(path);
    }
}

const std::string& Player::getType() const { return playerType; }

void Player::update(float dt, bool moving) {
    if (!moving) {
        sequenceIndex = 0;
        elapsed = 0.0f;
        return;
    }

    elapsed += dt;
    while (elapsed >= frameDuration) {
        elapsed -= frameDuration;
        sequenceIndex = (sequenceIndex + 1) % static_cast<int>(animationOrder.size());
    }
}

void Player::resetAnimation() {
    elapsed = 0.0f;
    sequenceIndex = 0;
}

void Player::drawAt(float boardX, float boardY, float tileSize, int row, int col) const {
    if (frames.empty()) {
        return;
    }

    int frameIndex = animationOrder[sequenceIndex];
    if (frameIndex < 0 || frameIndex >= static_cast<int>(frames.size()) || !frames[frameIndex].isLoaded()) {
        frameIndex = 0;
    }

    const float x = boardX + col * tileSize;
    const float y = boardY + row * tileSize;

    if (!frames[frameIndex].isLoaded()) {
        DrawCircle(static_cast<int>(x + tileSize / 2.0f), static_cast<int>(y + tileSize / 2.0f), tileSize * 0.35f, BLUE);
        return;
    }

    Rectangle src = AssetUtils::fullTextureSource(frames[frameIndex].get());
    Rectangle dst{x, y, tileSize, tileSize};
    DrawTexturePro(frames[frameIndex].get(), src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

Baby::Baby() : Player("Baby") {}
Fire::Fire() : Player("Fire") {}

std::unique_ptr<Player> createPlayerByType(const std::string& playerType) {
    if (playerType == "Fire") {
        return std::make_unique<Fire>();
    }
    return std::make_unique<Baby>();
}
