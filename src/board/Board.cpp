#include <iostream>
#include <cmath>
#include <stdexcept>
#include "board/Board.hpp"

using namespace std;

Board::Board(
    int rows,
    int cols,
    const vector<char>& tiles,
    const vector<int>& costs,
    Point startPosition,
    Point playerPosition,
    Point finishPosition,
    const vector<Point>& numberPositions
)
    : rows(rows),
      cols(cols),
      tiles(tiles),
      costs(costs),
      startPosition(startPosition),
      finishPosition(finishPosition),
      numberPositions(numberPositions) {}   

int Board::getRows() const {
    return rows;
}

int Board::getCols() const {
    return cols;
}

char Board::getTile(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw out_of_range("Tile position is outside the board");
    }

    return tiles[row * cols + col];
}

int Board::getCost(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw out_of_range("Tile position is outside the board");
    }
    return costs[row * cols + col];
}

const vector<Point>& Board::getNumberPositions() const {
    return numberPositions;
}

Point Board::getStartPosition() const {
    return startPosition;
}

Point Board::getFinishPosition() const {
    return finishPosition;
}

void Board::printBoard() const {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            cout << getTile(row, col);
        }
        cout << '\n';
    }
}

void Board::printBoardWithPlayer(Point playerPosition) const {
    if (playerPosition.x < 0 || playerPosition.x >= rows ||
        playerPosition.y < 0 || playerPosition.y >= cols) {
        throw out_of_range("Player position is outside the board");
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (row == playerPosition.x && col == playerPosition.y) {
                cout << 'P';
            } else {
                cout << getTile(row, col);
            }
        }
        cout << '\n';
    }
}

SlideResult Board::slideTo(Point start, Direction direction) const {
    int dx = 0;
    int dy = 0;

    switch (direction) {
        case Direction::up:
            dx = -1;
            dy = 0;
            break;
        case Direction::down:
            dx = 1;
            dy = 0;
            break;
        case Direction::left:
            dx = 0;
            dy = -1;
            break;
        case Direction::right:
            dx = 0;
            dy = 1;
            break;
    }

    Point current = start;
    int totalCost = 0;

    while (true) {
        Point next = {current.x + dx, current.y + dy};

        if (next.x < 0 || next.x >= rows || next.y < 0 || next.y >= cols) {
            return {
                current,
                true,
                totalCost
            };
        }

        char nextTile = getTile(next.x, next.y);

        if (nextTile == 'X') {
            return {
                current,
                false,
                totalCost
            };
        }

        if (nextTile == 'L') {
            return {
                next,
                true,
                totalCost
            };
        }

        current = next;
        totalCost += getCost(current.x, current.y);
    }
}
