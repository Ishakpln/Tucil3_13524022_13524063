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
    static float getPriority(const Node& node);
    bool isFinished(Point position) const;

    Result solve() override;
};


class HeapSet {
private:
    Node topNode;
    std::stack<Node> elements;
public:
    HeapSet(Node node);
    Node getTopValue();
    void pushElement(Node node);
    void popElement();
    bool isEmpty();
};


#endif
