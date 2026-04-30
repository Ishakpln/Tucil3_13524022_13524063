#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "utils/Helper.hpp"
#include <vector>
#include "board/Board.hpp"

struct State {
    Point position;
    int nextNumberTarget;
};

struct Node {
    State state;
    int gCost;
    int hCost;
    int parentIndex;
    Direction move;
};

struct Result { 
    bool found;
    int totalCost;
    int iterations;
    std::vector<Direction> movesSolution;
    std::vector<State> pathSolution;
    std::vector<State> expandedPaths;
};

class Solver {
protected:
    const Board& board;
public:
    Solver(const Board& board) : board(board){};
    virtual Result solve();
};

#endif