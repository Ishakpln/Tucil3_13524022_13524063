#include "solver/Algorithm/GBFS.hpp"
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>

GBFS::GBFS(const Board& board, HeuristicType heuristicType): Solver(board), heuristicType(heuristicType) {}

float GBFS::heuristic(Point position, int targetIdx) const
{
    return ::heuristic(position, targetIdx, heuristicType, board);
}

std::string GBFS::getStateKey(Node node) const
{
    return std::to_string(node.position.x) + "," +
           std::to_string(node.position.y) + "," +
           std::to_string(node.targetIndex);
}

Result GBFS::constructPath(const std::vector<Node>& allNodes, const Node& finalNode, const std::vector<Node>& expandedNodes, float duration)
{
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

Result GBFS::solve()
{
    auto sTime = std::chrono::high_resolution_clock::now();
    auto comparator = [](Node a, Node b) {return a.hCost > b.hCost;};
    std::priority_queue<Node, std::vector<Node>, decltype(comparator)> processQueue(comparator);
    std::unordered_map<std::string, int> visitedNode;
    std::vector<Node> allNodes;
    std::vector<Node> expandedNodes;
    Node start;
    start.position = board.getStartPosition();
    start.targetIndex = 0;
    start.gCost = 0;
    start.hCost = heuristic(start.position, start.targetIndex);
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

        if (visitedNode.find(key) != visitedNode.end()) {
            if (visitedNode[key] <= candidateNode.hCost) {
                continue;
            }
        }

        visitedNode[key] = candidateNode.hCost;

        int currIndex = static_cast<int>(allNodes.size());
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
            newNode.fCost = 0;
            newNode.move = d;

            if (slideResult.isFinished) {
                auto eTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float, std::milli> duration = eTime - sTime;
                allNodes.push_back(newNode);

                return constructPath(allNodes, newNode, expandedNodes, duration.count());
            }

            std::string newKey = getStateKey(newNode);

            if (visitedNode.find(newKey) != visitedNode.end()) {
                if (visitedNode[newKey] <= newNode.hCost) {
                    continue;
                }
            }

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

void GBFS::playSolution(Board board, const std::vector<Node>& pathSolution) {
    if (pathSolution.empty()) {
        std::cout << "no solution to display\n";
        return;
    }

    std::cout << "Initial:\n";
    board.printBoardWithPlayer(pathSolution[0].position);

    for (int i = 1; i < static_cast<int>(pathSolution.size()); i++) {
        const Node& node = pathSolution[i];

        std::cout << "\nStep " << i << " : " << node.move << '\n';
        std::cout << "H cost = " << node.hCost << '\n';
        board.printBoardWithPlayer(node.position);
    }
}

void GBFS::showSolutionAt(Board board, const std::vector<Node>& pathSolution, int index) {
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
    std::cout << "H cost = " << node.hCost << '\n';
    board.printBoardWithPlayer(node.position);
}

void GBFS::saveSolution(const std::string& outputPath, const Result& result) {
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
