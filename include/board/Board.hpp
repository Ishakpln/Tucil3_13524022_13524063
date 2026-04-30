#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "../include/utils/Helper.hpp"

class Board {
private:
    int rows;
    int cols;
    std::vector<char> tiles;
    Point startPosition;
    Point finishPosition;
    std::vector<Point> numberPositions;

public:
    Board(
        int rows,
        int cols,
        const std::vector<char>& tiles,
        Point startPosition,
        Point finishPosition,
        const std::vector<Point>& numberPositions
    );

    int getRows() const;
    int getCols() const;
    char getTile(int row, int col) const;
    float getDistance(Point start, Point target) const;
    SlideResult slideTo(Point start, Dir direction);
};

#endif
