#ifndef DFS_HPP
#define DFS_HPP

#include "Solver.hpp"
#include "utils/Helper.hpp"
#include <string>
#include <vector>

class DFS : public Solver {
private:
    std::string getStateKey(Node node) const;
    Result constructPath(
        const std::vector<Node>& allNodes,
        const Node& finalNode,
        const std::vector<Node>& expandedNodes,
        float duration
    ) const;

public:
    DFS(const Board& board);

    Result solve() override;
    void saveSolution(const std::string& outputPath, const Result& result) override;
};

#endif
