#ifndef UCS_HPP
#define UCS_HPP

#include "Solver.hpp"
#include "utils/Helper.hpp"
#include <vector>

class UCS : public Solver {
private:
    Result constructPath(const std::vector<Node>& allNodes, const Node& finalNode, const std::vector<Node>& expandedNodes);

public:
    UCS(const Board& board);

    Result solve() override;
};

#endif
