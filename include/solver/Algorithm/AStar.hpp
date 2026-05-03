#ifndef ASTAR_HPP
#define ASTAR_HPP

#include "Solver.hpp"
#include "solver/Heuristic/Heuristic.hpp"
#include "utils/Helper.hpp"
#include <queue>
#include <vector>

class AStar : public Solver {
private:
    HeuristicType heuristicType;
    Result constructPath(const std::vector<Node>& allNodes, const Node& finalNode, const std::vector<Node>& expandedNodes, float duration);
public:
    AStar(const Board& board, HeuristicType heuristicType);

    float heuristic(Point position, Point target) const;
    bool isFinished(Point position) const;

    Result solve() override;
    void saveSolution(const std::string& outputPath, const Result& result) override;
};


struct CompareNode {
    bool operator()(const Node& a, const Node& b) const {
        return a.fCost > b.fCost;
    }
};

class HeapSet {
private:
    std::priority_queue<Node, std::vector<Node>, CompareNode> elements;
public:
    HeapSet(Node node);
    Node getTopValue();
    void pushElement(Node node);
    void popElement();
    bool isEmpty();
};


#endif
