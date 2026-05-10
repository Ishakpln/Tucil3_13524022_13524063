#include "view/assets/Obstacle.hpp"
#include "view/assets/AssetUtils.hpp"
#include "utils/GuiHelper.hpp"
#include <utility>

Obstacle::Obstacle(std::string name, const std::string &texturePath)
    : texture(texturePath), assetName(std::move(name))
{
    if (texture.isLoaded())
    {
        widthInTiles = AssetUtils::dimensionToTileCount(texture.width(), texturePath, "width");
        heightInTiles = AssetUtils::dimensionToTileCount(texture.height(), texturePath, "height");
    }
}

const std::string &Obstacle::name() const { return assetName; }
bool Obstacle::isLoaded() const { return texture.isLoaded(); }
int Obstacle::getWidthInTiles() const { return widthInTiles; }
int Obstacle::getHeightInTiles() const { return heightInTiles; }

float Obstacle::getRotationFacingCenter(int, int, int, int) const
{
    return 0.0f;
}

void Obstacle::drawAt(float boardX, float boardY, float tileSize, int row, int col) const
{
    drawAt(boardX, boardY, tileSize, row, col, widthInTiles, heightInTiles, 0.0f);
}

void Obstacle::drawAt(float boardX, float boardY, float tileSize, int row, int col, int drawWidthTiles, int drawHeightTiles, float rotationDegrees) const
{
    const float x = boardX + col * tileSize;
    const float y = boardY + row * tileSize;
    const float w = drawWidthTiles * tileSize;
    const float h = drawHeightTiles * tileSize;

    if (!texture.isLoaded())
    {
        DrawRectangleLines(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h), Theme::Border);
        DrawText("X", static_cast<int>(x + w * 0.35f), static_cast<int>(y + h * 0.25f), static_cast<int>(tileSize * 0.5f), Theme::Border);
        return;
    }

    Rectangle src = AssetUtils::fullTextureSource(texture.get());

    if (rotationDegrees == 0.0f)
    {
        Rectangle dst{x, y, w, h};
        DrawTexturePro(texture.get(), src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    }
    else
    {
        Rectangle dst{x + w / 2.0f, y + h / 2.0f, w, h};
        Vector2 origin{w / 2.0f, h / 2.0f};
        DrawTexturePro(texture.get(), src, dst, origin, rotationDegrees, WHITE);
    }
}
