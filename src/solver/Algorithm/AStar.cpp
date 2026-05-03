#include "solver/Algorithm/AStar.hpp"
#include <algorithm>
#include <fstream>
#include <stdexcept>
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

Result AStar::constructPath(const std::vector<Node>& allNodes, const Node& finalNode, const std::vector<Node>& expandedNodes, float duration) {
    Result result;
    result.found = true;
    result.time = duration;
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
    auto start = std::chrono::high_resolution_clock::now();
    Node startNode = {board.getStartPosition(), 0, 0, 
                    heuristic(board.getStartPosition(), board.getFinishPosition()),
                    heuristic(board.getStartPosition(), board.getFinishPosition()),
                    -1, Direction::U};
    
    HeapSet openNodes = HeapSet(startNode);
    std::unordered_map<std::string, float> closeNodes;
    std::vector<Node> allNodes;
    std::vector<Node> expandedNodes;

    Direction directions[] = {
        Direction::U,
        Direction::D,
        Direction::L,
        Direction::R
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
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float, std::milli> duration = end - start;
                allNodes.push_back(newNode);

                return constructPath(allNodes, newNode, expandedNodes, duration.count());
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

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end - start;
    Result result;
    result.found = false;
    result.time = duration.count();
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

void AStar::saveSolution(const std::string& outputPath, const Result& result) {
    std::ofstream output(outputPath);

    if (!output.is_open()) {
        throw std::runtime_error("Failed to open output file: " + outputPath);
    }

    output << board.getRows() << ' ' << board.getCols() << '\n';

    for (int row = 0; row < board.getRows(); row++) {
        for (int col = 0; col < board.getCols(); col++) {
            output << board.getTile({row, col});
        }
        output << '\n';
    }

    for (int row = 0; row < board.getRows(); row++) {
        for (int col = 0; col < board.getCols(); col++) {
            if (col > 0) {
                output << ' ';
            }

            output << board.getCost({row, col});
        }
        output << '\n';
    }

    output << '\n';

    if (!result.found || result.pathSolution.empty()) {
        output << "No solution found\n";
        output << "Cost total = 0\n";
        return;
    }

    for (Direction move : result.movesSolution) {
        output << move;
    }

    output << "\n";
    output << "cost awal = " << board.getCost(result.pathSolution[0].position) << " \n";

    for (int i = 1; i < static_cast<int>(result.pathSolution.size()); i++) {
        const Node& previousNode = result.pathSolution[i - 1];
        const Node& currentNode = result.pathSolution[i];
        Direction move = currentNode.move;

        int dx = 0;
        int dy = 0;

        switch (move) {
            case Direction::U:
                dx = -1;
                break;
            case Direction::D:
                dx = 1;
                break;
            case Direction::L:
                dy = -1;
                break;
            case Direction::R:
                dy = 1;
                break;
        }

        Point position = previousNode.position;
        std::vector<int> moveCosts;

        while (position != currentNode.position) {
            position = {
                position.x + dx,
                position.y + dy
            };

            moveCosts.push_back(board.getCost(position));
        }

        output << move << " -> cost = ";

        if (moveCosts.empty()) {
            output << 0;
        }
        else {
            for (int costIndex = 0; costIndex < static_cast<int>(moveCosts.size()); costIndex++) {
                if (costIndex > 0) {
                    output << " + ";
                }

                output << moveCosts[costIndex];
            }
        }

        output << '\n';
    }

    output << "Cost total = " << result.totalCost << '\n';
}
