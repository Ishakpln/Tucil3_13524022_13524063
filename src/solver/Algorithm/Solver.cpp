#include "solver/Algorithm/Solver.hpp"
#include <iostream>

Solver::Solver(const Board& board) : board(board) {}

Solver::~Solver() = default;

void Solver::playSolution(Board board, const std::vector<Node>& pathSolution) {
    if (pathSolution.empty()) {
        std::cout << "no solution to display\n";
        return;
    }

    std::cout << "Initial:\n";
    board.printBoardWithPlayer(pathSolution[0].position);

    for (int i = 1; i <pathSolution.size(); i++) {
        const Node& node = pathSolution[i];
        std::cout << "\nStep " << i << " : " << pathSolution[i].move << '\n';
        std::cout << "G cost = " << node.gCost << '\n';
        board.printBoardWithPlayer(node.position);
    }
}

void Solver::showSolutionAt(Board board, const std::vector<Node>& pathSolution, int index) {
    if (pathSolution.empty()) {
        std::cout << "no solution to display\n";
        return;
    }  

    const Node& node = pathSolution[index];
    std::cout << "Step " << index;
    if (index > 0) std::cout << " : " << node.move << "\n";
    std::cout << "G cost = " << node.gCost << "\n";
    board.printBoardWithPlayer(pathSolution[index].position);
}