#include "view/assets/BoardRenderer.hpp"
#include "view/assets/AssetUtils.hpp"
#include "view/assets/Obstacles.hpp"
#include "utils/GuiHelper.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>

BoardRenderer::BoardRenderer(const std::string& type):
    playerType(type.empty() ? "Baby" : type),
    floorTexture("./assets/components/FloorTile.png"),
    goalCheckpoint(playerType),
    player(createPlayerByType(playerType)) {
    loadObstacleCatalog();
}

void BoardRenderer::setPlayerType(const std::string& newPlayerType) {
    playerType = newPlayerType.empty() ? "Baby" : newPlayerType;
    goalCheckpoint.reloadForPlayer(playerType);
    player = createPlayerByType(playerType);
}

void BoardRenderer::loadObstacleCatalog() {
    obstacleCatalog.clear();

    // Order matters for deterministic fallback when several assets have the same tile size.
    obstacleCatalog.push_back(std::make_unique<Bed>());
    obstacleCatalog.push_back(std::make_unique<DinnerTable>());
    obstacleCatalog.push_back(std::make_unique<TV>());
    obstacleCatalog.push_back(std::make_unique<Desk>());
    obstacleCatalog.push_back(std::make_unique<Sofa>(2));
    obstacleCatalog.push_back(std::make_unique<SofaCoffe>());
    obstacleCatalog.push_back(std::make_unique<Sofa>(1));
    obstacleCatalog.push_back(std::make_unique<Plant>());
    obstacleCatalog.push_back(std::make_unique<Ball>(1));
    obstacleCatalog.push_back(std::make_unique<Ball>(2));
    obstacleCatalog.push_back(std::make_unique<Ball>(3));

    obstacleCatalog.erase(
        std::remove_if(obstacleCatalog.begin(), obstacleCatalog.end(),
                       [](const std::unique_ptr<Obstacle>& obstacle) { return !obstacle->isLoaded(); }),
        obstacleCatalog.end()
    );

    std::stable_sort(obstacleCatalog.begin(), obstacleCatalog.end(),
                     [](const std::unique_ptr<Obstacle>& a, const std::unique_ptr<Obstacle>& b) {
                         int areaA = a->getWidthInTiles() * a->getHeightInTiles();
                         int areaB = b->getWidthInTiles() * b->getHeightInTiles();
                         if (areaA != areaB) return areaA > areaB;
                         if (a->getHeightInTiles() != b->getHeightInTiles()) return a->getHeightInTiles() > b->getHeightInTiles();
                         return a->getWidthInTiles() > b->getWidthInTiles();
                     });
}

void BoardRenderer::update(float dt, bool animatePlayer) {
    if (player) {
        player->update(dt, animatePlayer || player->usesContinuousAnimation());
    }
}

BoardLayout BoardRenderer::calculateLayout(const Board& board, Rectangle bounds) const {
    BoardLayout layout;

    if (board.getRows() <= 0 || board.getCols() <= 0) {
        layout.x = bounds.x;
        layout.y = bounds.y;
        layout.tileSize = 32.0f;
        return layout;
    }

    const float tileByWidth = bounds.width / static_cast<float>(board.getCols());
    const float tileByHeight = bounds.height / static_cast<float>(board.getRows());
    layout.tileSize = std::max(8.0f, std::min(tileByWidth, tileByHeight));

    const float boardWidth = layout.tileSize * board.getCols();
    const float boardHeight = layout.tileSize * board.getRows();
    layout.x = bounds.x + (bounds.width - boardWidth) / 2.0f;
    layout.y = bounds.y + (bounds.height - boardHeight) / 2.0f;

    return layout;
}

std::vector<ObstacleRegion> BoardRenderer::buildGreedyObstacleRegions(const Board& board) const {
    return buildGreedyObstacleRegions(
        board.getRows(),
        board.getCols(),
        [&board](int row, int col) { return board.getTile(Point{row, col}); }
    );
}

std::vector<ObstacleRegion> BoardRenderer::buildGreedyObstacleRegions(int rows, int cols, const std::function<char(int, int)>& tileAt) const {
    std::vector<ObstacleRegion> regions;

    if (rows <= 0 || cols <= 0) {
        return regions;
    }

    std::vector<std::vector<bool>> used(rows, std::vector<bool>(cols, false));

    auto isFreeObstacle = [&](int row, int col) {
        return row >= 0 && row < rows && col >= 0 && col < cols &&
               !used[row][col] && tileAt(row, col) == 'X';
    };

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (!isFreeObstacle(row, col)) {
                continue;
            }

            int bestWidth = 1;
            int bestHeight = 1;
            int bestArea = 1;
            int minWidth = 0;

            for (int h = 1; row + h - 1 < rows; ++h) {
                int currentWidth = 0;
                while (col + currentWidth < cols && isFreeObstacle(row + h - 1, col + currentWidth)) {
                    ++currentWidth;
                }

                if (currentWidth == 0) {
                    break;
                }

                minWidth = (h == 1) ? currentWidth : std::min(minWidth, currentWidth);
                const int area = minWidth * h;

                if (area > bestArea ||
                    (area == bestArea && h > bestHeight) ||
                    (area == bestArea && h == bestHeight && minWidth > bestWidth)) {
                    bestArea = area;
                    bestWidth = minWidth;
                    bestHeight = h;
                }
            }

            for (int r = row; r < row + bestHeight; ++r) {
                for (int c = col; c < col + bestWidth; ++c) {
                    used[r][c] = true;
                }
            }

            regions.push_back(ObstacleRegion{row, col, bestWidth, bestHeight});
        }
    }

    return regions;
}

const Obstacle* BoardRenderer::chooseExactObstacle(const ObstacleRegion& region) const {
    std::vector<const Obstacle*> matches;

    for (const auto& obstacle : obstacleCatalog) {
        if (obstacle->getWidthInTiles() == region.width && obstacle->getHeightInTiles() == region.height) {
            matches.push_back(obstacle.get());
        }
    }

    if (matches.empty()) {
        return nullptr;
    }

    // A stable hash gives variety among same-sized assets without making replays nondeterministic.
    const std::size_t key = static_cast<std::size_t>(
        region.row * 73856093 ^ region.col * 19349663 ^ region.width * 83492791 ^ region.height * 2654435761u
    );
    return matches[key % matches.size()];
}

const Obstacle* BoardRenderer::chooseFittingObstacle(int widthTiles, int heightTiles, int row, int col) const {
    std::vector<const Obstacle*> matches;
    int bestArea = 0;

    for (const auto& obstacle : obstacleCatalog) {
        if (obstacle->getWidthInTiles() <= widthTiles && obstacle->getHeightInTiles() <= heightTiles) {
            int area = obstacle->getWidthInTiles() * obstacle->getHeightInTiles();
            if (area > bestArea) {
                matches.clear();
                bestArea = area;
            }
            if (area == bestArea) {
                matches.push_back(obstacle.get());
            }
        }
    }

    if (matches.empty()) {
        return nullptr;
    }

    const std::size_t key = static_cast<std::size_t>(
        row * 73856093 ^ col * 19349663 ^ widthTiles * 83492791 ^ heightTiles * 2654435761u
    );
    return matches[key % matches.size()];
}

void BoardRenderer::drawObstacleRegion(const ObstacleRegion& region, int boardRows, int boardCols, const BoardLayout& layout) const {
    const Obstacle* exact = chooseExactObstacle(region);

    if (exact != nullptr) {
        float rotation = exact->getRotationFacingCenter(region.row, region.col, boardRows, boardCols);
        exact->drawAt(layout.x, layout.y, layout.tileSize, region.row, region.col, region.width, region.height, rotation);
        return;
    }

    std::vector<std::vector<bool>> covered(region.height, std::vector<bool>(region.width, false));

    auto canPlace = [&](const Obstacle* obstacle, int localRow, int localCol) {
        if (obstacle == nullptr) return false;
        if (localRow + obstacle->getHeightInTiles() > region.height) return false;
        if (localCol + obstacle->getWidthInTiles() > region.width) return false;

        for (int r = localRow; r < localRow + obstacle->getHeightInTiles(); ++r) {
            for (int c = localCol; c < localCol + obstacle->getWidthInTiles(); ++c) {
                if (covered[r][c]) return false;
            }
        }
        return true;
    };

    for (int localRow = 0; localRow < region.height; ++localRow) {
        for (int localCol = 0; localCol < region.width; ++localCol) {
            if (covered[localRow][localCol]) {
                continue;
            }

            const Obstacle* fallback = chooseFittingObstacle(region.width - localCol, region.height - localRow, region.row + localRow, region.col + localCol);
            if (!canPlace(fallback, localRow, localCol)) {
                fallback = nullptr;
            }

            if (fallback == nullptr) {
                const float x = layout.x + (region.col + localCol) * layout.tileSize;
                const float y = layout.y + (region.row + localRow) * layout.tileSize;
                DrawRectangleLines(static_cast<int>(x), static_cast<int>(y), static_cast<int>(layout.tileSize), static_cast<int>(layout.tileSize), DARKGRAY);
                covered[localRow][localCol] = true;
                continue;
            }

            float rotation = fallback->getRotationFacingCenter(region.row + localRow, region.col + localCol, boardRows, boardCols);
            fallback->drawAt(layout.x, layout.y, layout.tileSize, region.row + localRow, region.col + localCol,

                fallback->getWidthInTiles(), fallback->getHeightInTiles(), rotation);

            for (int r = localRow; r < localRow + fallback->getHeightInTiles(); ++r) {
                for (int c = localCol; c < localCol + fallback->getWidthInTiles(); ++c) {
                    covered[r][c] = true;
                }
            }
        }
    }
}

void BoardRenderer::drawBoard(const Board& board, Rectangle bounds, Point playerPosition, bool drawPlayer, float playerRotationDegrees) const {
    drawBoardAt(board, bounds, static_cast<float>(playerPosition.x), static_cast<float>(playerPosition.y), drawPlayer, playerRotationDegrees);
}

void BoardRenderer::drawBoardAt(const Board& board, Rectangle bounds, float playerRow, float playerCol, bool drawPlayer, float playerRotationDegrees) const {
    if (board.getRows() <= 0 || board.getCols() <= 0) {
        DrawText("No board loaded", static_cast<int>(bounds.x + 20), static_cast<int>(bounds.y + 20), 24, GRAY);
        return;
    }

    float safePlayerRow = playerRow;
    float safePlayerCol = playerCol;
    if (!std::isfinite(safePlayerRow) || !std::isfinite(safePlayerCol) ||
        safePlayerRow < 0.0f || safePlayerRow >= static_cast<float>(board.getRows()) ||
        safePlayerCol < 0.0f || safePlayerCol >= static_cast<float>(board.getCols())) {
        Point start = board.getStartPosition();
        safePlayerRow = static_cast<float>(start.x);
        safePlayerCol = static_cast<float>(start.y);
    }

    drawTileMap(
        board.getRows(),
        board.getCols(),
        [&board](int row, int col) { return board.getTile(Point{row, col}); },
        bounds,
        safePlayerRow,
        safePlayerCol,
        drawPlayer,
        playerRotationDegrees,
        true
    );
}

void BoardRenderer::drawEditorBoard(int rows, int cols, const std::vector<char>& tiles, Rectangle bounds, bool drawPlayer) const {
    if (rows <= 0 || cols <= 0 || static_cast<int>(tiles.size()) < rows * cols) {
        DrawText("Invalid board draft", static_cast<int>(bounds.x + 20), static_cast<int>(bounds.y + 20), 18, Theme::Error);
        return;
    }

    float playerRow = -1.0f;
    float playerCol = -1.0f;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (tiles[row * cols + col] == 'Z') {
                playerRow = static_cast<float>(row);
                playerCol = static_cast<float>(col);
            }
        }
    }

    drawTileMap(
        rows,
        cols,
        [&tiles, cols](int row, int col) { return tiles[row * cols + col]; },
        bounds,
        playerRow,
        playerCol,
        drawPlayer && playerRow >= 0.0f,
        0.0f,
        false
    );
}

void BoardRenderer::drawTileMap(int rows, int cols, const std::function<char(int, int)>& tileAt, Rectangle bounds,
                                float playerRow, float playerCol, bool drawPlayer, float playerRotationDegrees,
                                bool drawGrid) const {
    BoardLayout layout;
    if (rows <= 0 || cols <= 0) {
        layout.x = bounds.x;
        layout.y = bounds.y;
        layout.tileSize = 32.0f;
    }
    else {
        const float tileByWidth = bounds.width / static_cast<float>(cols);
        const float tileByHeight = bounds.height / static_cast<float>(rows);
        layout.tileSize = std::max(8.0f, std::min(tileByWidth, tileByHeight));

        const float boardWidth = layout.tileSize * cols;
        const float boardHeight = layout.tileSize * rows;
        layout.x = bounds.x + (bounds.width - boardWidth) / 2.0f;
        layout.y = bounds.y + (bounds.height - boardHeight) / 2.0f;
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            const float x = layout.x + col * layout.tileSize;
            const float y = layout.y + row * layout.tileSize;
            Rectangle dst{x, y, layout.tileSize, layout.tileSize};

            if (floorTexture.isLoaded()) {
                DrawTexturePro(floorTexture.get(), AssetUtils::fullTextureSource(floorTexture.get()), dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            }
            else {
                DrawRectangleRec(dst, Theme::Background);
            }
        }
    }

    for (const ObstacleRegion& region : buildGreedyObstacleRegions(rows, cols, tileAt)) {
        drawObstacleRegion(region, rows, cols, layout);
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            const char tile = tileAt(row, col);
            const float x = layout.x + col * layout.tileSize;
            const float y = layout.y + row * layout.tileSize;

            if (tile == 'L') {
                DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(layout.tileSize), static_cast<int>(layout.tileSize), Fade(Theme::AccentDark, 0.6f));
            }
            else if (tile >= '0' && tile <= '9') {
                goalCheckpoint.drawCheckpoint(layout.x, layout.y, layout.tileSize, row, col, tile);
            }
            else if (tile == 'O') {
                goalCheckpoint.drawGoal(layout.x, layout.y, layout.tileSize, row, col);
            }
        }
    }

    if (drawPlayer && player) {
        float safeRow = playerRow;
        float safeCol = playerCol;
        if (!std::isfinite(safeRow) || !std::isfinite(safeCol) ||
            safeRow < 0.0f || safeRow >= static_cast<float>(rows) ||
            safeCol < 0.0f || safeCol >= static_cast<float>(cols)) {
            drawPlayer = false;
        }
        if (drawPlayer) {
            const float rotation = player->usesMovementRotation() ? playerRotationDegrees : 0.0f;
            player->drawAtTilePosition(layout.x, layout.y, layout.tileSize, safeRow, safeCol, rotation);
        }
    }

    if (drawGrid) {
        for (int row = 0; row <= rows; ++row) {
            DrawLine(static_cast<int>(layout.x), static_cast<int>(layout.y + row * layout.tileSize),
                     static_cast<int>(layout.x + cols * layout.tileSize), static_cast<int>(layout.y + row * layout.tileSize), Fade(Theme::Grid, 0.15f));
        }
        for (int col = 0; col <= cols; ++col) {
            DrawLine(static_cast<int>(layout.x + col * layout.tileSize), static_cast<int>(layout.y),
                     static_cast<int>(layout.x + col * layout.tileSize), static_cast<int>(layout.y + rows * layout.tileSize), Fade(Theme::Grid, 0.15f));
        }
    }
}
