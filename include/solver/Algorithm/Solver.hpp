#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "utils/Helper.hpp"
#include <vector>
#include <stack>
#include <queue>
#include "board/Board.hpp"
#include <chrono>
#include <string>

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
    Solver(const Board& board);
    virtual ~Solver();
    virtual Result solve() = 0;
    virtual void playSolution(Board board, const std::vector<Node>& pathSolution);
    virtual void showSolutionAt(Board board, const std::vector<Node>& pathSolution, int iteration);
    virtual void saveSolution(const std::string& outputPath, const Result& result) = 0;
};

#endif
