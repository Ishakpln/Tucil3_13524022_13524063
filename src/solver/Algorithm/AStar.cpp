#include "solver/Algorithm/AStar.hpp"
#include <unordered_map>

AStar::AStar(Board board, HeuristicType heuristicType)
    : Solver(board), heuristicType(heuristicType) {}

float AStar::heuristic(Point position, Point target) const {
    return ::heuristic(position, target, HeuristicType::EUCLIDEAN);
}

float AStar::getPriority(const Node& node) {
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

std::string getStateKey(const Node& node) {
    return std::to_string(node.position.x) + "," +
           std::to_string(node.position.y) + "," +
           std::to_string(node.targetIndex);
}

Result AStar::solve() {
    Node startNode = {board.getStartPosition(), 0, 0, 
                    heuristic(board.getStartPosition(), board.getFinishPosition()),
                    heuristic(board.getStartPosition(), board.getFinishPosition()),
                    -1, Direction::up};
    
    HeapSet openNodes = HeapSet(startNode);
    std::unordered_map<std::string, float> closeNodes;
    SlideResult slideResult = {startNode.position, false, false, 0};    

    Direction directions[] = {
        Direction::up,
        Direction::down,
        Direction::left,
        Direction::right
    };

    while (!openNodes.isEmpty()) {
        Node candidateNode = openNodes.getTopValue();

        if (slideResult.isFinished) {
            return; //implement reconstruksi path
        }

        openNodes.popElement();
        std::string key = getStateKey(candidateNode);
        closeNodes[key] = candidateNode.fCost;

        for (Direction d : directions) {
            slideResult = board.slideTo(candidateNode, d);
            candidateNode = {slideResult.position, slideResult.targetIndex, 
                            candidateNode.gCost + slideResult.cost, 
                            heuristic(slideResult.position,board.getFinishPosition()),
                            0, /*ga ngerti isi parent index*/, d};
        }



    }

    return {};
}

HeapSet::HeapSet(Node node) : topNode(node) {
    elements.push(node);
};

Node HeapSet::getTopValue() {
    return topNode;
}
void HeapSet::pushElement(Node node) {
    if (AStar::getPriority(getTopValue()) < AStar::getPriority(topNode)) { //semakin rendah semakin ke atas
        topNode = node;
    }
    elements.push(node);
} 

void HeapSet::popElement() {
    if (!isEmpty()) {
        elements.pop();
    }
    else {
        return;
    }
}

bool HeapSet::isEmpty() {
    if (elements.empty()) {
        return true;
    }
    else {
        return false;
    }
}
