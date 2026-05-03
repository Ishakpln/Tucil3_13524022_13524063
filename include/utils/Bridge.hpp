#ifndef BRIDGE_HPP
#define BRIDGE_HPP

#include "board/Board.hpp"
#include "utils/Helper.hpp"
#include <string>
#include <vector>

struct SolveResult {
    AlgorithmType algorithm;
    HeuristicType heuristic;
    bool isFound;
    int cost;
    std::string moves;
    float execTime;
    int iterations;
    std::vector<Node> pathSolution;
};

std::vector<HeuristicType> getAllHeuristics();
std::vector<AlgorithmType> getAllAlgorithms();
SolveResult solve(const std::string& filename, AlgorithmType algorithm, HeuristicType heuristic);
SolveResult solve(const Board& board, AlgorithmType algorithm, HeuristicType heuristic);
void playSolution(const std::string& inputPath, const SolveResult& result);
void playSolution(const Board& board, const SolveResult& result);
void showSolutionAt(const std::string& inputPath, const SolveResult& result, int iteration);
void showSolutionAt(const Board& board, const SolveResult& result, int iteration);
void saveSolution(const std::string& inputPath, const std::string& outputPath, AlgorithmType algorithm, HeuristicType heuristic);
void saveSolution(const Board& board, const std::string& outputPath, AlgorithmType algorithm, HeuristicType heuristic);

#endif
