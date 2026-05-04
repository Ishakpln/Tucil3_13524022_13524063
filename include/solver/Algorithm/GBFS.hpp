#ifndef GBFS_HPP
#define GBFS_HPP

#include "Solver.hpp"
#include "solver/Heuristic/Heuristic.hpp"
#include "utils/Helper.hpp"
#include <vector>

class GBFS : public Solver {
private:
    HeuristicType heuristicType;
    std::string getStateKey(Node node) const;
    Result constructPath(const std::vector<Node>& allNodes, const Node& finalNode, const std::vector<Node>& expandedNodes, float duration);

public:
    GBFS(const Board& board, HeuristicType heuristicType);

    float heuristic(Point position, int targetIdx) const;
    Result solve() override;
    void playSolution(Board board, const std::vector<Node>& pathSolution) override;
    void showSolutionAt(Board board, const std::vector<Node>& pathSolution, int index) override;
    void saveSolution(const std::string& outputPath, const Result& result) override;
};

#endif
