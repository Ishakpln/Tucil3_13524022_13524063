#include "view/assets/GoalCheckpoint.hpp"
#include "view/assets/AssetUtils.hpp"

GoalCheckpoint::GoalCheckpoint(const std::string& type) {
    reloadForPlayer(type);
}

void GoalCheckpoint::reloadForPlayer(const std::string& newPlayerType) {
    playerType = newPlayerType.empty() ? "Baby" : newPlayerType;
    const std::string base = "./assets/components/GoalCheckpoint/" + playerType + "/";

    // The uploaded asset currently has a typo: Checkponit.png. Keep the correct name first.
    checkpointTexture.load(AssetUtils::firstExistingPath({base + "Checkpoint.png", base + "Checkponit.png"}));
    goalTexture.load(AssetUtils::firstExistingPath({base + "Goal.png"}));
}

void GoalCheckpoint::drawCheckpoint(float boardX, float boardY, float tileSize, int row, int col, char label) const {
    const float x = boardX + col * tileSize;
    const float y = boardY + row * tileSize;

    if (checkpointTexture.isLoaded()) {
        DrawTexturePro(
            checkpointTexture.get(),
            AssetUtils::fullTextureSource(checkpointTexture.get()),
            Rectangle{x, y, tileSize, tileSize},
            Vector2{0.0f, 0.0f},
            0.0f,
            WHITE
        );
    }
    else {
        DrawCircle(static_cast<int>(x + tileSize / 2.0f), static_cast<int>(y + tileSize / 2.0f), tileSize * 0.35f, GOLD);
    }

    DrawText(TextFormat("%c", label), static_cast<int>(x + tileSize * 0.38f), static_cast<int>(y + tileSize * 0.32f),
             static_cast<int>(tileSize * 0.35f), BLACK);
}

void GoalCheckpoint::drawGoal(float boardX, float boardY, float tileSize, int row, int col) const {
    const float x = boardX + col * tileSize;
    const float y = boardY + row * tileSize;

    if (goalTexture.isLoaded()) {
        DrawTexturePro(
            goalTexture.get(),
            AssetUtils::fullTextureSource(goalTexture.get()),
            Rectangle{x, y, tileSize, tileSize},
            Vector2{0.0f, 0.0f},
            0.0f,
            WHITE
        );
    }
    else {
        DrawCircle(static_cast<int>(x + tileSize / 2.0f), static_cast<int>(y + tileSize / 2.0f), tileSize * 0.35f, GREEN);
        DrawText("O", static_cast<int>(x + tileSize * 0.35f), static_cast<int>(y + tileSize * 0.25f),
                 static_cast<int>(tileSize * 0.45f), BLACK);
    }
}
