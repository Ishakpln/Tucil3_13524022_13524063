#include <iostream>
#include <cmath>
#include <stdexcept>
#include "board/Board.hpp"
#include <limits>
#include <queue>
#include <unordered_map>

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
    const vector<CheckpointDistances>& checkpointDistances
)
    : rows(rows),
      cols(cols),
      tiles(tiles),
      costs(costs),
      startPosition(startPosition),
      finishPosition(finishPosition),
      checkpointDistances(checkpointDistances) {

        float total = 0;
        int count = 0;
        int minimumCost = std::numeric_limits<int>::max();
        for (int i = 0;i < static_cast<int>(tiles.size());i++) {
            if (tiles.at(i) == '*') {
                total += costs.at(i);
                if (costs.at(i) <= minimumCost) {
                    minimumCost = costs.at(i);
                }
                count++;
            }
        }

        avgCost = count > 0 ? total/count : 0;
        minCost = count > 0 ? minimumCost : 0;

        countCheckpointDist();
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
    CheckpointDistances checkpoint = checkpointDistances.at(index);

    if (checkpoint.index < 0) {
        return 'O';
    }

    return static_cast<char>('0' + checkpoint.index);
}

Point Board::getNumberPosition(int index) const {
    return checkpointDistances.at(index).position;
}

int Board::getCheckpointCount() const {
    return static_cast<int>(checkpointDistances.size());
}

CheckpointDistances Board::getCheckpointDistance(int index) const {
    return checkpointDistances.at(index);
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

void Board::countCheckpointDist() {
    struct QueueEntry {
        Node node;
        int cost;
    };

    struct CompareQueueEntry {
        bool operator()(const QueueEntry& a, const QueueEntry& b) const {
            return a.cost > b.cost;
        }
    };

    auto getStateKey = [](const Node& node) {
        return std::to_string(node.position.x) + "," +
               std::to_string(node.position.y) + "," +
               std::to_string(node.targetIndex);
    };

    auto isTargetReached = [this](const Node& node, int nextCheckpointIndex) {
        if (node.targetIndex > nextCheckpointIndex) {
            return true;
        }

        return checkpointDistances[nextCheckpointIndex].index < 0 &&
               node.position == checkpointDistances[nextCheckpointIndex].position;
    };

    Direction directions[] = {
        Direction::U,
        Direction::D,
        Direction::L,
        Direction::R
    };

    for (int i = 0; i + 1 < static_cast<int>(checkpointDistances.size()); i++) {
        Node startNode;
        startNode.position = checkpointDistances[i].position;
        startNode.targetIndex = i + 1;
        startNode.gCost = 0;
        startNode.hCost = 0;
        startNode.fCost = 0;
        startNode.parentIndex = -1;
        startNode.move = Direction::U;

        std::priority_queue<QueueEntry, std::vector<QueueEntry>, CompareQueueEntry> processQueue;
        std::unordered_map<std::string, int> bestCost;

        processQueue.push({startNode, 0});

        checkpointDistances[i].euclideanDist = getEuclideanDist(
            checkpointDistances[i].position,
            checkpointDistances[i + 1].position,
            minCost
        );
        checkpointDistances[i].manhattanDist = getManhattanDist(
            checkpointDistances[i].position,
            checkpointDistances[i + 1].position,
            minCost
        );
        checkpointDistances[i].realDist = static_cast<int>(checkpointDistances[i].manhattanDist);

        while (!processQueue.empty()) {
            QueueEntry current = processQueue.top();
            processQueue.pop();

            std::string currentKey = getStateKey(current.node);

            if (bestCost.find(currentKey) != bestCost.end() &&
                bestCost[currentKey] <= current.cost) {
                continue;
            }

            bestCost[currentKey] = current.cost;

            if (isTargetReached(current.node, i + 1)) {
                checkpointDistances[i].realDist = current.cost;
                break;
            }

            for (Direction direction : directions) {
                SlideResult slideResult = slideTo(current.node, direction);

                if (slideResult.isGameOver) {
                    continue;
                }

                Node nextNode;
                nextNode.position = slideResult.position;
                nextNode.targetIndex = slideResult.targetIndex;
                nextNode.gCost = current.cost + slideResult.cost;
                nextNode.hCost = 0;
                nextNode.fCost = 0;
                nextNode.parentIndex = -1;
                nextNode.move = direction;

                std::string nextKey = getStateKey(nextNode);

                if (bestCost.find(nextKey) != bestCost.end() &&
                    bestCost[nextKey] <= nextNode.gCost) {
                    continue;
                }

                processQueue.push({nextNode, nextNode.gCost});
            }
        }
    }

    if (!checkpointDistances.empty()) {
        checkpointDistances.back().euclideanDist = 0;
        checkpointDistances.back().manhattanDist = 0;
        checkpointDistances.back().realDist = 0;
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
                targetIndex < static_cast<int>(checkpointDistances.size()) &&
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
            if (targetIndex >= static_cast<int>(checkpointDistances.size()) || getNumber(targetIndex) != 'O') {
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
            if (targetIndex >= static_cast<int>(checkpointDistances.size())) {
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
