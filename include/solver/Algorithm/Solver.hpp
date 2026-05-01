#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "utils/Helper.hpp"
#include <vector>
#include <stack>
#include <queue>
#include "board/Board.hpp"

struct Result { 
    bool found;
    int totalCost;
    int iterations;
    std::vector<Direction> movesSolution;
    std::vector<Point> pathSolution;
    std::vector<Point> expandedPaths;
};

class Solver {
protected:
    const Board& board;
    std::queue<Point> targetPositions;
    Point startPosition;
public:
    Solver(const Board& board) : board(board){};
    virtual Result solve();
};

#endif