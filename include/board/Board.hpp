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
    Point startPosition;
    Point finishPosition;
    std::vector<Point> numberPositions;

public:
    Board(
        int rows,
        int cols,
        const std::vector<char>& tiles,
        const std::vector<int>& costs,
        Point startPosition,
        Point playerPosition,
        Point finishPosition,
        const std::vector<Point>& numberPositions
    );

    int getRows() const;
    int getCols() const;
    char getTile(int row, int col) const;
    int getCost(int row, int col) const;
    float getDistance(Point start, Point target) const;
    void printBoard() const;
    void printBoardWithPlayer(Point playerPosition) const;
    SlideResult slideTo(Point start, Direction direction) const;
};

#endif
