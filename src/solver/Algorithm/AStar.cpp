#include "solver/Algorithm/AStar.hpp"

AStar::AStar(Board board, HeuristicType heuristicType)
    : Solver(board), heuristicType(heuristicType) {}

float AStar::heuristic(Point position, Point target) const {
    return ::heuristic(position, target, HeuristicType::EUCLIDEAN);
}

float AStar::getPriority(const Node& node) const {
    return node.gCost + node.hCost;
}

bool AStar::isFinished(Point position) const {
    if (board.getTile(position) == 'O') {
        return true;
    }
    else {
        return false;
    }

}

Result AStar::solve() {
    std::vector<Node> setNode;
    Node startNode = {board.getStartPosition(), 0, 0, 
                    heuristic(board.getStartPosition(), board.getNumberPositions(0)),
                     -1, Direction::up};
    setNode.push_back(startNode);

    Direction directions[] = {
        Direction::up,
        Direction::down,
        Direction::left,
        Direction::right
    };

while (!setNode.empty()) {

}

    return {};
}
