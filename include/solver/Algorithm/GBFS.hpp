#ifndef GBFS_HPP
#define GBFS_HPP

#include "Solver.hpp"
#include "solver/Heuristic/Heuristic.hpp"
#include "utils/Helper.hpp"
#include <vector>

class GBFS : public Solver {
private:
    HeuristicType heuristicType;
    Result constructPath(const std::vector<Node>& allNodes, const Node& finalNode, const std::vector<Node>& expandedNodes);

public:
    GBFS(const Board& board, HeuristicType heuristicType);

    float heuristic(Point position, Point target) const;
    Result solve() override;
};

#endif
