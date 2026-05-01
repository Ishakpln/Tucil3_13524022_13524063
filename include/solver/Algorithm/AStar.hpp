#ifndef ASTAR_HPP
#define ASTAR_HPP

#include "Solver.hpp"
#include "solver/Heuristic/Heuristic.hpp"
#include "utils/Helper.hpp"

class AStar : public Solver {
private:
    HeuristicType heuristicType;
public:
    AStar(Board board, HeuristicType heuristicType);

    float heuristic(Point position, Point target) const;
    float getPriority(const Node& node) const;
    bool isFinished(Point position) const;

    Result solve() override;
};

#endif
