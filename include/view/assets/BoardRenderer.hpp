#pragma once

#include "board/Board.hpp"
#include "raylib.h"
#include "view/assets/GoalCheckpoint.hpp"
#include "view/assets/Obstacle.hpp"
#include "view/assets/Player.hpp"
#include "view/assets/TextureResource.hpp"
#include <memory>
#include <string>
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

    void loadObstacleCatalog();
    const Obstacle* findExactObstacle(int widthTiles, int heightTiles) const;
    const Obstacle* findLargestFittingObstacle(int widthTiles, int heightTiles) const;
    void drawObstacleRegion(const ObstacleRegion& region, const Board& board, const BoardLayout& layout) const;

public:
    explicit BoardRenderer(const std::string& playerType = "Baby");

    void setPlayerType(const std::string& newPlayerType);
    void update(float dt, bool animatePlayer = true);

    BoardLayout calculateLayout(const Board& board, Rectangle bounds) const;
    std::vector<ObstacleRegion> buildGreedyObstacleRegions(const Board& board) const;
    void drawBoard(const Board& board, Rectangle bounds, Point playerPosition, bool drawPlayer = true) const;
};
