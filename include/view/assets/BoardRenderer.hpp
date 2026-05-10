#pragma once

#include "board/Board.hpp"
#include "raylib.h"
#include "view/assets/GoalCheckpoint.hpp"
#include "view/assets/Obstacle.hpp"
#include "view/assets/Player.hpp"
#include "view/assets/TextureResource.hpp"
#include <functional>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

struct BoardLayout {
    float x = 0.0f;
    float y = 0.0f;
    float tileSize = 32.0f;
};

struct ObstacleRegion {
    int row = 0;
    int col = 0;
    int width = 1;
    int height = 1;
};

class BoardRenderer {
private:
    std::string playerType;
    TextureResource floorTexture;
    GoalCheckpoint goalCheckpoint;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Obstacle>> obstacleCatalog;
    mutable std::mt19937 randomEngine;
    mutable std::unordered_map<std::size_t, std::size_t> obstacleChoiceCache;

    void loadObstacleCatalog();
    std::size_t cachedRandomChoice(std::size_t key, std::size_t optionCount) const;
    const Obstacle* chooseExactObstacle(const ObstacleRegion& region) const;
    const Obstacle* chooseFittingObstacle(int widthTiles, int heightTiles, int row, int col) const;
    std::vector<ObstacleRegion> buildGreedyObstacleRegions(int rows, int cols, const std::function<char(int, int)>& tileAt) const;
    void drawObstacleRegion(const ObstacleRegion& region, int boardRows, int boardCols, const BoardLayout& layout) const;
    void drawTileMap(int rows, int cols, const std::function<char(int, int)>& tileAt, Rectangle bounds,
                     float playerRow, float playerCol, bool drawPlayer, float playerRotationDegrees,
                     bool drawGrid, int completedCheckpointCount) const;

public:
    explicit BoardRenderer(const std::string& playerType = "Baby");

    void setPlayerType(const std::string& newPlayerType);
    void update(float dt, bool animatePlayer = true);

    BoardLayout calculateLayout(const Board& board, Rectangle bounds) const;
    std::vector<ObstacleRegion> buildGreedyObstacleRegions(const Board& board) const;
    void drawBoard(const Board& board, Rectangle bounds, Point playerPosition, bool drawPlayer = true, float playerRotationDegrees = 0.0f) const;
    void drawBoardAt(const Board& board, Rectangle bounds, float playerRow, float playerCol, bool drawPlayer = true, float playerRotationDegrees = 0.0f) const;
    void drawBoardAt(const Board& board, Rectangle bounds, float playerRow, float playerCol, bool drawPlayer, float playerRotationDegrees, int completedCheckpointCount) const;
    void drawEditorBoard(int rows, int cols, const std::vector<char>& tiles, Rectangle bounds, bool drawPlayer = true) const;
};
