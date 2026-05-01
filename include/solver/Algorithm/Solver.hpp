#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "utils/Helper.hpp"
#include <vector>
#include <stack>
#include <queue>
#include "board/Board.hpp"

struct Result { 
    bool found;
    float time;
    int totalCost;
    int iterations;
    std::vector<Direction> movesSolution;
    std::vector<Node> pathSolution;
    std::vector<Node> expandedPaths;
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