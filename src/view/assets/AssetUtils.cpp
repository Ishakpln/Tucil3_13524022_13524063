#include "view/assets/AssetUtils.hpp"
#include <algorithm>
#include <cmath>

namespace AssetUtils {
    int dimensionToTileCount(int pixels, const std::string& path, const char* axisName) {
        if (pixels <= 0) {
            TraceLog(LOG_WARNING, "Invalid %s dimension for %s; using 1 tile", axisName, path.c_str());
            return 1;
        }

        int count = std::max(1, static_cast<int>(std::lround(
            static_cast<double>(pixels) / SOURCE_PIXELS_PER_TILE
        )));
        int expectedPixels = count * SOURCE_PIXELS_PER_TILE;

        if (std::abs(pixels - expectedPixels) > TILE_TOLERANCE_PIXELS) {
            TraceLog(
                LOG_WARNING,
                "Texture %s %s=%d is not within +/- %d px of %d; using %d tile(s)",
                path.c_str(),
                axisName,
                pixels,
                TILE_TOLERANCE_PIXELS,
                expectedPixels,
                count
            );
        }

        return count;
    }

    std::string firstExistingPath(const std::initializer_list<std::string>& candidates) {
        for (const std::string& path : candidates) {
            if (FileExists(path.c_str())) {
                return path;
            }
        }

        return candidates.size() > 0 ? *candidates.begin() : std::string{};
    }

    Rectangle fullTextureSource(const Texture2D& texture) {
        return Rectangle{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
    }
}
