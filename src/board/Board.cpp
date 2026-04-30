#include <iostream>
#include <cmath>
#include <stdexcept>
#include "../include/board/Board.hpp"

Board::Board(
    int rows,
    int cols,
    const std::vector<char>& tiles,
    Point startPosition,
    Point finishPosition,
    const std::vector<Point>& numberPositions
)
    : rows(rows),
      cols(cols),
      tiles(tiles),
      startPosition(startPosition),
      finishPosition(finishPosition),
      numberPositions(numberPositions) {}   x

int Board::getRows() const {
    return rows;
}

int Board::getCols() const {
    return cols;
}

char Board::getTile(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("Tile position is outside the board");
    }

    return tiles[row * cols + col];
}

float Board::getDistance(Point start, Point target) const {
    int dx = target.x - start.x;
    int dy = target.y - start.y;
    return std::sqrt(dx * dx + dy * dy);
}

SlideResult Board::slideTo(Point start, Dir direction) {
    // returns struct SlideResult after input up/left/right/down from initial position start
}
