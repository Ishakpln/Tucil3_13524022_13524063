#ifndef BFS_HPP
#define BFS_HPP

#include "Solver.hpp"
#include "utils/Helper.hpp"
#include <string>
#include <vector>

class BFS : public Solver {
private:
    std::string getStateKey(Node node) const;
    Result constructPath(
        const std::vector<Node>& allNodes,
        const Node& finalNode,
        const std::vector<Node>& expandedNodes,
        float duration
    ) const;

public:
    BFS(const Board& board);

    Result solve() override;
    void saveSolution(const std::string& outputPath, const Result& result) override;
};

#endif
