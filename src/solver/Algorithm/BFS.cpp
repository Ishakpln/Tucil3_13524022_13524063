#include "solver/Algorithm/BFS.hpp"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <queue>
#include <stdexcept>
#include <unordered_set>

BFS::BFS(const Board& board) : Solver(board) {}

std::string BFS::getStateKey(Node node) const {
    return std::to_string(node.position.x) + "," +
           std::to_string(node.position.y) + "," +
           std::to_string(node.targetIndex);
}

Result BFS::constructPath(
    const std::vector<Node>& allNodes,
    const Node& finalNode,
    const std::vector<Node>& expandedNodes,
    float duration
) const {
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

Result BFS::solve() {
    auto sTime = std::chrono::high_resolution_clock::now();
    std::queue<Node> processQueue;
    std::unordered_set<std::string> visitedNodes;
    std::vector<Node> allNodes;
    std::vector<Node> expandedNodes;

    Node start;
    start.position = board.getStartPosition();
    start.targetIndex = 0;
    start.gCost = 0;
    start.hCost = 0;
    start.fCost = 0;
    start.parentIndex = -1;
    start.move = Direction::U;

    processQueue.push(start);
    visitedNodes.insert(getStateKey(start));

    Direction directions[] = {
        Direction::U,
        Direction::D,
        Direction::L,
        Direction::R
    };

    while (!processQueue.empty()) {
        Node candidateNode = processQueue.front();
        processQueue.pop();

        int currIndex = static_cast<int>(allNodes.size());
        allNodes.push_back(candidateNode);
        expandedNodes.push_back(candidateNode);

        for (Direction dir : directions) {
            SlideResult slideResult = board.slideTo(candidateNode, dir);

            if (slideResult.isGameOver) {
                continue;
            }

            Node newNode;
            newNode.position = slideResult.position;
            newNode.targetIndex = slideResult.targetIndex;
            newNode.parentIndex = currIndex;
            newNode.gCost = candidateNode.gCost + slideResult.cost;
            newNode.hCost = 0;
            newNode.fCost = 0;
            newNode.move = dir;

            if (slideResult.isFinished) {
                auto eTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float, std::milli> duration = eTime - sTime;
                return constructPath(allNodes, newNode, expandedNodes, duration.count());
            }

            std::string newKey = getStateKey(newNode);

            if (visitedNodes.find(newKey) != visitedNodes.end()) {
                continue;
            }

            visitedNodes.insert(newKey);
            processQueue.push(newNode);
        }
    }

    auto eTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = eTime - sTime;

    Result result;
    result.found = false;
    result.time = duration.count();
    result.totalCost = 0;
    result.iterations = static_cast<int>(expandedNodes.size());
    result.expandedPaths = expandedNodes;

    return result;
}

void BFS::saveSolution(const std::string& outputPath, const Result& result) {
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
