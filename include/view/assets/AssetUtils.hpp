#pragma once

#include "raylib.h"
#include <initializer_list>
#include <string>

namespace AssetUtils {
    constexpr int SOURCE_PIXELS_PER_TILE = 600;
    constexpr int TILE_TOLERANCE_PIXELS = 10;

    int dimensionToTileCount(int pixels, const std::string& path, const char* axisName);
    std::string firstExistingPath(const std::initializer_list<std::string>& candidates);
    Rectangle fullTextureSource(const Texture2D& texture);
}
