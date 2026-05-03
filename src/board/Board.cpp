#include <iostream>
#include <cmath>
#include <stdexcept>
#include "board/Board.hpp"
#include <limits>

using namespace std;
Board::Board(){};
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
      numbersTarget(numbersTarget) {

        float total = 0;
        int count = 0;
        int minCost = std::numeric_limits<int>::max();
        for (int i = 0;i < tiles.size();i++) {
            if (tiles.at(i) == '*') {
                total += costs.at(i);
                if (costs.at(i) <= minCost) {
                    minCost = costs.at(i);
                }
                count++;
            }
        }

        avgCost = count > 0 ? total/count : 0;
        minCost = count > 0 ? minCost : 0;
      }   

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

float Board::getAvgCost() const {
    return avgCost;
}

int Board::getMinCost() const {
    return minCost;
}

char Board::getNumber(int index) const {
    return numbersTarget[index];
}

Point Board::getNumberPosition(int index) const {
    return numberPositions[index];
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
                cout << 'Z';
            }
            else {
                char tile = getTile({row, col});

                if (tile == 'Z') {
                    cout << '*';
                }
                else {
                    cout << tile;
                }
            }
        }
        cout << '\n';
    }
}

SlideResult Board::slideTo(Node start, Direction direction) const {
    int dx = 0;
    int dy = 0;

    switch (direction) {
        case Direction::U:
            dx = -1;
            break;
        case Direction::D:
            dx = 1;
            break;
        case Direction::L:
            dy = -1;
            break;
        case Direction::R:
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
                currPoint,
                targetIndex,
                true,
                false,
                totalCost
            };
        }

        char nextTile = getTile(nextPoint);

        if (nextTile == 'X') {
            bool isFinished = false;

            if (getTile(currPoint) == 'O' &&
                targetIndex < numbersTarget.size() &&
                getNumber(targetIndex) == 'O') {
                isFinished = true;
            }

            return {
                currPoint,
                targetIndex,
                false,
                isFinished,
                totalCost
            };
        }

        if (nextTile == 'L') {
            return {
                currPoint,
                targetIndex,
                true,
                false,
                totalCost
            };
        }

        if (nextTile == 'O') {
            if (targetIndex >= numbersTarget.size() || getNumber(targetIndex) != 'O') {
                return {
                    currPoint,
                    targetIndex,
                    true,
                    false,
                    totalCost
                };
            }
        }

        if (nextTile >= '0' && nextTile <= '9') {
            if (targetIndex >= numbersTarget.size()) {
                return {
                    currPoint,
                    targetIndex,
                    true,
                    false,
                    totalCost
                };
            }

            char targetNumber = getNumber(targetIndex);

            if (targetNumber >= '0' && targetNumber <= '9') {
                if (nextTile - '0' > targetNumber - '0') {
                    return {
                        currPoint,
                        targetIndex,
                        true,
                        false,
                        totalCost
                    };
                }

                if (nextTile == targetNumber) {
                    targetIndex++;
                }
            }

        }

        currPoint = nextPoint;
        totalCost += getCost(nextPoint);
    }
}
