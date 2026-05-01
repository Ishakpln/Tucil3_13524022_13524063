#include "solver/Algorithm/AStar.hpp"
#include <unordered_map>

AStar::AStar(Board board, HeuristicType heuristicType)
    : Solver(board), heuristicType(heuristicType) {}

float AStar::heuristic(Point position, Point target) const {
    return ::heuristic(position, target, HeuristicType::EUCLIDEAN);
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

Result AStar::constructPath(std::vector<Node> allNodes, Node finalNode) {
    
}

Result AStar::solve() {
    Node startNode = {board.getStartPosition(), 0, 0, 
                    heuristic(board.getStartPosition(), board.getFinishPosition()),
                    heuristic(board.getStartPosition(), board.getFinishPosition()),
                    -1, Direction::up};
    
    HeapSet openNodes = HeapSet(startNode);
    std::unordered_map<std::string, float> closeNodes;
    std::vector<Node> allNodes;
    allNodes.push_back(startNode);
    SlideResult slideResult = {startNode.position, false, false, 0};    

    Direction directions[] = {
        Direction::up,
        Direction::down,
        Direction::left,
        Direction::right
    };

    while (!openNodes.isEmpty()) {
        Node candidateNode = openNodes.getTopValue();
        openNodes.popElement();

        std::string key = getStateKey(candidateNode);

        if (closeNodes.find(key) != closeNodes.end()) {
            if (closeNodes[key] <= candidateNode.gCost) {
                continue;
            }
        }

        closeNodes[key] = candidateNode.gCost;

        int currIndex = allNodes.size();
        allNodes.push_back(candidateNode);

        
        for (Direction d : directions) {
            SlideResult slideResult = board.slideTo(candidateNode, d);

            if (slideResult.isGameOver) {
                continue;
            }

            Node newNode;
            newNode.position = slideResult.position;
            newNode.targetIndex = slideResult.targetIndex;
            newNode.parentIndex = currIndex;
            newNode.gCost = candidateNode.gCost + slideResult.cost;
            newNode.hCost = heuristic(newNode.position, board.getFinishPosition());
            newNode.fCost = newNode.gCost + newNode.hCost;
            newNode.move = d;

            if (slideResult.isFinished) {
                allNodes.push_back(newNode);

                return constructPath(allNodes, newNode);
            }

            std::string newKey = getStateKey(newNode);

            if (closeNodes.find(newKey) != closeNodes.end()) {
                if (closeNodes[newKey] <= newNode.gCost) {
                    continue;
                }
            }

            openNodes.pushElement(newNode);
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
    if (getTopValue().fCost < node.fCost) { //semakin rendah semakin ke atas
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
