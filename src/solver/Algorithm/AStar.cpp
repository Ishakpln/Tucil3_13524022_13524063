#include "solver/Algorithm/AStar.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

AStar::AStar(const Board& board, HeuristicType heuristicType)
    : Solver(board), heuristicType(heuristicType) {}

float AStar::heuristic(Point position, int targetIndex) const {
    return ::heuristic(position, targetIndex, heuristicType, board);
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
                    heuristic(board.getStartPosition(), 0),
                    heuristic(board.getStartPosition(), 0),
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
            newNode.hCost = heuristic(newNode.position, newNode.targetIndex);
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

void AStar::playSolution(Board board, const std::vector<Node>& pathSolution) {
    if (pathSolution.empty()) {
        std::cout << "no solution to display\n";
        return;
    }

    std::cout << "Initial:\n";
    board.printBoardWithPlayer(pathSolution[0].position);

    for (int i = 1; i < static_cast<int>(pathSolution.size()); i++) {
        const Node& node = pathSolution[i];

        std::cout << "\nStep " << i << " : " << node.move << '\n';
        std::cout << "G cost = " << node.gCost << '\n';
        std::cout << "H cost = " << node.hCost << '\n';
        std::cout << "F cost = " << node.fCost << "\n\n";
        board.printBoardWithPlayer(node.position);
    }
}

void AStar::showSolutionAt(Board board, const std::vector<Node>& pathSolution, int index) {
    if (pathSolution.empty()) {
        std::cout << "no solution to display\n";
        return;
    }

    const Node& node = pathSolution[index];

    std::cout << "Step " << index;

    if (index > 0) {
        std::cout << " : " << node.move;
    }

    std::cout << '\n';
    std::cout << "G cost = " << node.gCost << '\n';
    std::cout << "H cost = " << node.hCost << '\n';
    std::cout << "F cost = " << node.fCost << "\n\n";
    board.printBoardWithPlayer(node.position);
}

void AStar::saveSolution(const std::string& outputPath, const Result& result) {
    std::ofstream output(outputPath);

    if (!output.is_open()) {
        throw std::runtime_error("Failed to open output file: " + outputPath);
    }

    output << "Problem:\n";
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

    output << "\nSolution:\n";

    if (!result.found || result.pathSolution.empty()) {
        output << "Move solution : -\n";
        output << "Total cost    : 0\n";
        return;
    }

    output << "Move solution : ";
    for (Direction move : result.movesSolution) {
        output << move;
    }

    output << '\n';
    output << "Total cost    : " << result.totalCost << '\n';
}
