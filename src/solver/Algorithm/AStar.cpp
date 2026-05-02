#include "solver/Algorithm/AStar.hpp"
#include <algorithm>
#include <unordered_map>

AStar::AStar(const Board& board, HeuristicType heuristicType)
    : Solver(board), heuristicType(heuristicType) {}

float AStar::heuristic(Point position, Point target) const {
    return ::heuristic(position, target, heuristicType);
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

Result AStar::constructPath(const std::vector<Node>& allNodes, const Node& finalNode, const std::vector<Node>& expandedNodes) {
    Result result;
    result.found = true;
    result.time = 0;
    result.totalCost = finalNode.gCost;
    result.iterations = static_cast<int>(expandedNodes.size());
    result.expandedPaths = expandedNodes;

    Node currentNode = finalNode;

    while (true) {
        result.pathSolution.push_back(currentNode);

        if (currentNode.parentIndex == -1) {
            break;
        }

        currentNode = allNodes[currentNode.parentIndex];
    }

    std::reverse(result.pathSolution.begin(), result.pathSolution.end());

    for (int i = 1; i < static_cast<int>(result.pathSolution.size()); i++) {
        result.movesSolution.push_back(result.pathSolution[i].move);
    }

    return result;
}

Result AStar::solve() {
    Node startNode = {board.getStartPosition(), 0, 0, 
                    heuristic(board.getStartPosition(), board.getFinishPosition()),
                    heuristic(board.getStartPosition(), board.getFinishPosition()),
                    -1, Direction::up};
    
    HeapSet openNodes = HeapSet(startNode);
    std::unordered_map<std::string, float> closeNodes;
    std::vector<Node> allNodes;
    std::vector<Node> expandedNodes;

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
        expandedNodes.push_back(candidateNode);

        
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

                return constructPath(allNodes, newNode, expandedNodes);
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

    Result result;
    result.found = false;
    result.time = 0;
    result.totalCost = 0;
    result.iterations = static_cast<int>(expandedNodes.size());
    result.expandedPaths = expandedNodes;

    return result;
}

HeapSet::HeapSet(Node node) {
    elements.push(node);
};

Node HeapSet::getTopValue() {
    return elements.top();
}

void HeapSet::pushElement(Node node) {
    elements.push(node);
} 

void HeapSet::popElement() {
    if (!elements.empty()) {
        elements.pop();
    }
}

bool HeapSet::isEmpty() {
    return elements.empty();
}
