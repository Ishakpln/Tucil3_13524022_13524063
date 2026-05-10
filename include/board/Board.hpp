#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "utils/Helper.hpp"

class Board {
private:
    int rows;
    int cols;
    std::vector<char> tiles;
    std::vector<int> costs;
    float avgCost;
    int minCost;
    Point startPosition;
    Point finishPosition;
    std::vector<CheckpointDistances> checkpointDistances;
    void countCheckpointDist();

public:
    Board();
    Board(
        int rows,
        int cols,
        const std::vector<char>& tiles,
        const std::vector<int>& costs,
        Point startPosition,
        Point playerPosition,
        Point finishPosition,
        const std::vector<CheckpointDistances>& checkpointDistances
    );

    int getRows() const;
    int getCols() const;
    Point getNumberPosition(int index) const;
    Point getStartPosition() const;
    Point getFinishPosition() const;
    char getTile(Point point) const;
    Point getTilePosition(char c) const;
    int getCost(Point point) const;
    float getAvgCost() const;
    int getMinCost() const;
    char getNumber(int index) const;
    int getCheckpointCount() const;
    CheckpointDistances getCheckpointDistance(int index) const;
    void printBoard() const;
    void printBoardWithPlayer(Point playerPosition) const;
    SlideResult slideTo(Node start, Direction direction) const;
};

#endif
