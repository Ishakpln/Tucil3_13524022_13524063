#include "utils/Loader.hpp"
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace {
    bool isBlankLine(const std::string& line) {
        for (char c : line) {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                return false;
            }
        }

        return true;
    }

    bool isValidTile(char tile) {
        return tile == '*' ||
               tile == 'X' ||
               tile == 'L' ||
               tile == 'Z' ||
               tile == 'O' ||
               (tile >= '0' && tile <= '9');
    }

    std::string readNextNonEmptyLine(std::ifstream& input, const std::string& context) {
        std::string line;

        while (std::getline(input, line)) {
            if (!isBlankLine(line)) {
                return line;
            }
        }

        throw std::invalid_argument("Invalid input: missing " + context);
    }
}

Board loadBoard(const std::string& filename) {
    std::ifstream input(filename);

    if (!input.is_open()) {
        throw std::runtime_error("Failed to open input file: " + filename);
    }

    int rows;
    int cols;

    if (!(input >> rows >> cols)) {
        throw std::invalid_argument("Invalid input: first line must contain board rows and columns");
    }

    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Invalid input: board rows and columns must be positive");
    }

    std::string line;
    std::getline(input, line);

    std::vector<char> tiles;
    tiles.reserve(rows * cols);

    Point startPosition = {-1, -1};
    Point finishPosition = {-1, -1};
    std::vector<bool> numberExists(10, false);
    std::vector<Point> digitPositions(10, {-1, -1});
    int startCount = 0;
    int finishCount = 0;

    for (int row = 0; row < rows; row++) {
        line = readNextNonEmptyLine(input, "board row " + std::to_string(row + 1));

        std::vector<char> rowTiles;
        rowTiles.reserve(cols);

        for (char tile : line) {
            if (std::isspace(static_cast<unsigned char>(tile))) {
                continue;
            }

            rowTiles.push_back(tile);
        }

        if (static_cast<int>(rowTiles.size()) != cols) {
            throw std::invalid_argument("Invalid input: board row " + std::to_string(row + 1) +
                                        " must contain exactly " + std::to_string(cols) + " tiles");
        }

        for (int col = 0; col < cols; col++) {
            char tile = rowTiles[col];

            if (!isValidTile(tile)) {
                throw std::invalid_argument("Invalid input: invalid tile '" + std::string(1, tile) +
                                            "' at row " + std::to_string(row + 1) +
                                            ", column " + std::to_string(col + 1));
            }

            if (tile == 'Z') {
                startPosition = {row, col};
                startCount++;
            }
            else if (tile == 'O') {
                finishPosition = {row, col};
                finishCount++;
            }
            else if (tile >= '0' && tile <= '9') {
                int digit = tile - '0';

                if (numberExists[digit]) {
                    throw std::invalid_argument("Invalid input: duplicate number tile '" +
                                                std::string(1, tile) + "'");
                }

                numberExists[digit] = true;
                digitPositions[digit] = {row, col};
            }

            tiles.push_back(tile);
        }
    }

    if (startCount != 1) {
        throw std::invalid_argument("Invalid input: board must contain exactly one start tile 'Z'");
    }

    if (finishCount != 1) {
        throw std::invalid_argument("Invalid input: board must contain exactly one finish tile 'O'");
    }

    int highestNumber = -1;

    for (int digit = 9; digit >= 0; digit--) {
        if (numberExists[digit]) {
            highestNumber = digit;
            break;
        }
    }

    std::vector<CheckpointDistances> checkpointDistances;

    if (highestNumber != -1) {
        for (int digit = 0; digit <= highestNumber; digit++) {
            if (!numberExists[digit]) {
                throw std::invalid_argument("Invalid input: number tiles must start from 0 and be contiguous");
            }

            checkpointDistances.push_back({
                digit,
                digitPositions[digit],
                0,
                0,
                0
            });
        }
    }

    checkpointDistances.push_back({
        -1,
        finishPosition,
        0,
        0,
        0
    });

    std::vector<int> costs;
    costs.reserve(rows * cols);

    for (int i = 0; i < rows * cols; i++) {
        int cost;

        if (!(input >> cost)) {
            throw std::invalid_argument("Invalid input: expected " + std::to_string(rows * cols) + " tile costs");
        }

        if (cost < 0) {
            throw std::invalid_argument("Invalid input: tile costs must be non-negative");
        }

        costs.push_back(cost);
    }

    std::string extraToken;

    if (input >> extraToken) {
        throw std::invalid_argument("Invalid input: unexpected extra token '" + extraToken + "'");
    }

    return Board(
        rows,
        cols,
        tiles,
        costs,
        startPosition,
        startPosition,
        finishPosition,
        checkpointDistances
    );
}
