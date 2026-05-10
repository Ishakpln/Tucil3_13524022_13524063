#include "solver/Algorithm/UCS.hpp"
#include <queue>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <limits>

UCS::UCS(const Board& board): Solver(board) {}

std::string UCS::getStateKey(Node node)
{
    return std::to_string(node.position.x) + "," +
           std::to_string(node.position.y) + "," +
           std::to_string(node.targetIndex);
}

Result UCS::constructPath(const std::vector<Node>& allNodes, const Node& finalNode, const std::vector<Node>& expandedNodes, float duration) {
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

Result UCS::solve()
{

    auto sTime = std::chrono::high_resolution_clock::now();
    auto comparator = [](Node a, Node b){return a.gCost > b.gCost;};
    std::priority_queue<Node, std::vector<Node>, decltype(comparator)> processQueue(comparator);
    std::unordered_map<std::string, int> visitedNode;
    std::vector<Node> allNodes;
    std::vector<Node> expandedNodes;
    bool checkAnotherNode = false;
    Node temporaryFinalNode;
    int temporaryFinalCost = std::numeric_limits<int>::max();

    Result result;
    Node start;
    start.position = board.getStartPosition();
    start.targetIndex = 0;
    start.gCost = 0;
    start.hCost = 0;
    start.fCost = 0;
    start.parentIndex = -1;
    start.move = Direction::U;

    processQueue.push(start);

    Direction directions[] = {
        Direction::U,
        Direction::D,
        Direction::L,
        Direction::R
    };

    while (!processQueue.empty())
    {
        Node candidateNode = processQueue.top();
        processQueue.pop();
        std::string key = getStateKey(candidateNode);

        if (visitedNode.find(key) != visitedNode.end())
        {
            if (visitedNode[key] <= candidateNode.gCost) continue;
        }

        visitedNode[key] = candidateNode.gCost;

        int currIndex = allNodes.size();
        allNodes.push_back(candidateNode);
        expandedNodes.push_back(candidateNode);

        for (auto dir: directions)
        {
            SlideResult sr = board.slideTo(candidateNode, dir);
            if (sr.isGameOver) continue;

            Node newNode;
            newNode.position = sr.position;
            newNode.targetIndex = sr.targetIndex;
            newNode.parentIndex = currIndex;
            newNode.gCost = candidateNode.gCost + sr.cost;
            newNode.hCost = 0;
            newNode.fCost = 0;
            newNode.move = dir;

            if (sr.isFinished) {
                if (newNode.gCost < temporaryFinalCost) {
                    temporaryFinalCost = newNode.gCost;
                    temporaryFinalNode = newNode;
                    checkAnotherNode = true;
                }

                if (!processQueue.empty() && processQueue.top().gCost < temporaryFinalCost)
                {
                    continue;
                }

                auto eTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float, std::milli> duration = eTime - sTime;
                return constructPath(allNodes, temporaryFinalNode, expandedNodes, duration.count());
            }

            std::string newKey = getStateKey(newNode);
            if (visitedNode.find(newKey) != visitedNode.end())
            {
                if (visitedNode[newKey] <= newNode.gCost) continue;
            }
            processQueue.push(newNode);
        }
    }

    auto eTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = eTime - sTime;

    if (checkAnotherNode) {
        return constructPath(allNodes, temporaryFinalNode, expandedNodes, duration.count());
    }

    result.found = false;
    result.time = duration.count();
    result.totalCost = 0;
    result.iterations = static_cast<int>(expandedNodes.size());
    result.expandedPaths = expandedNodes;

    return result;
}

void UCS::saveSolution(const std::string& outputPath, const Result& result) {
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



