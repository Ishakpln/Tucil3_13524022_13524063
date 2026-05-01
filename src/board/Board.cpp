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
    const vector<Point>& numberPositions,
    const vector<char>& numbersTarget
)
    : rows(rows),
      cols(cols),
      tiles(tiles),
      costs(costs),
      startPosition(startPosition),
      finishPosition(finishPosition),
      numberPositions(numberPositions),
      numbersTarget(numbersTarget) {}   

int Board::getRows() const {
    return rows;
}

int Board::getCols() const {
    return cols;
}

char Board::getTile(Point point) const {
    if (point.x < 0 || point.x >= rows || point.y < 0 || point.y >= cols) {
        throw out_of_range("Tile position is outside the board");
    }

    return tiles[point.x * cols + point.y];
}

Point Board::getTilePosition(char c) const {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Point position = {row, col};

            if (getTile(position) == c) {
                return position;
            }
        }
    }

    throw invalid_argument("Tile character does not exist on the board");
}

int Board::getCost(Point point) const {
    if (point.x < 0 || point.x >= rows || point.y < 0 || point.y >= cols) {
        throw out_of_range("Tile position is outside the board");
    }
    return costs[point.x * cols + point.y];
}

char Board::getNumber(int index) const {
    return numbersTarget[index];
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
            cout << getTile({row, col});
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
                cout << getTile({row, col});
            }
        }
        cout << '\n';
    }
}

SlideResult Board::slideTo(Node start, Direction direction) const {
    int dx = 0;
    int dy = 0;

    switch (direction) {
        case Direction::up:
            dx = -1;
            break;
        case Direction::down:
            dx = 1;
            break;
        case Direction::left:
            dy = -1;
            break;
        case Direction::right:
            dy = 1;
            break;
    }

    int totalCost = 0;
    int targetIndex = start.targetIndex;
    Point currPoint = start.position;

    while (true) {
        Point nextPoint = {
            currPoint.x + dx,
            currPoint.y + dy
        };

        if (nextPoint.x < 0 || nextPoint.x >= rows ||
            nextPoint.y < 0 || nextPoint.y >= cols) {
            return {
                start,
                true,
                false,
                totalCost
            };
        }

        char nextTile = getTile(nextPoint);

        if (nextTile == 'X') {
            Node newNode = {
                currPoint,
                targetIndex,
                0,
                0,
                0,
                direction
            };

            bool isFinished =
                getTile(currPoint) == 'O' &&
                getNumber(targetIndex) == 'O';

            return {
                newNode,
                false,
                isFinished,
                totalCost
            };
        }

        if (nextTile == 'L') {
            return {
                start,
                true,
                false,
                totalCost
            };
        }

        if (nextTile >= '0' && nextTile <= '9') {
            char targetNumber = getNumber(targetIndex);

            if (nextTile - '0' > targetNumber - '0') {
                return {
                    start,
                    true,
                    false,
                    totalCost
                };
            }

            if (nextTile == targetNumber) {
                targetIndex++;
            }
        }

        currPoint = nextPoint;
        totalCost += getCost(nextPoint);
    }
}