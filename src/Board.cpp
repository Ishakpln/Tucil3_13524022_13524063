#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include "Helper.hpp"

class Board {
    private:
        int rows;
        int cols;
        std::vector<char> tiles;
        Point startPosition;
        Point finishPosition;
        std::vector<Point> numberPositions;

    public:
        Board(int rows, int cols, const std::vector<char>& tiles, Point startPosition,
            Point finishPosition, const std::vector<Point>& numberPositions)
             : rows(rows), cols(cols), tiles(tiles), startPosition(startPosition),
             finishPosition(finishPosition), numberPositions(numberPositions) {}
        
        int getRows() const {
            return rows;
        }
        int getCols() const {
            return cols;
        }
        char getTile(int row, int col) const {
            if (row < 0 || row >= rows || col < 0 || col >= cols) {
                throw "index out of range";
            }

            return tiles[row * cols + col];
        }

        float getDistance(Point start, Point target) const {
            int dx = target.x - start.x;
            int dy = target.y - start.y;
            return std::sqrt(dx * dx + dy * dy);
        }

        SlideResult slideTo(Point start, Dir direction) {
            //returns struct SlideResult after input up/left/right/down from initial position start
        };

};
