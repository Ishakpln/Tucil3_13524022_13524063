#ifndef UCS_HPP
#define UCS_HPP

#include "Solver.hpp"
#include "utils/Helper.hpp"
#include <vector>

class UCS : public Solver {
private:
    Result constructPath(const std::vector<Node>& allNodes, const Node& finalNode, const std::vector<Node>& expandedNodes, float duration);
    std::string getStateKey(Node node);

public:
    UCS(const Board& board);
    Result solve() override;
    void saveSolution (const std::string& outputPath, const Result& result) override;

};

#endif
