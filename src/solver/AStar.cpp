#include "solver/Algorithm/AStar.hpp"

AStar::AStar(const Board& board, HeuristicType heuristicType)
    : Solver(board), heuristicType(heuristicType) {}

Result AStar::solve() {
    return {};
}
