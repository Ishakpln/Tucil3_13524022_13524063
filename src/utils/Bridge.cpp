#include "utils/Bridge.hpp"

#include "solver/Algorithm/AStar.hpp"
#include "solver/Algorithm/UCS.hpp"
#include "solver/Algorithm/GBFS.hpp"
#include "utils/Loader.hpp"
#include <memory>
#include <sstream>
#include <stdexcept>

namespace {
    std::unique_ptr<Solver> createSolver(AlgorithmType algorithm, const Board& board, HeuristicType heuristic) {
        switch (algorithm) {
            case AlgorithmType::ASTAR:
                return std::make_unique<AStar>(board, heuristic);
            case AlgorithmType::UCS:
                return std::make_unique<UCS>(board);
            case AlgorithmType::GBFS:
                return std::make_unique<GBFS>(board, heuristic);
            case AlgorithmType::BFS:
                throw std::runtime_error("belum implementasi bfs");
            case AlgorithmType::DFS:
                throw std::runtime_error("belum implementasi dfs");
            case AlgorithmType::SENTINEL:
                break;
        }

        throw std::runtime_error("Invalid Algorithm");
    }

    std::string movesToString(const std::vector<Direction>& moves) {
        if (moves.empty()) {
            return "-";
        }

        std::ostringstream output;

        for (Direction move : moves) {
            output << move;
        }

        return output.str();
    }
}

std::vector<HeuristicType> getAllHeuristics() {
    std::vector<HeuristicType> result;
    for (int i = 0; i < static_cast<int>(HeuristicType::SENTINEL); ++i) {
        result.push_back(static_cast<HeuristicType>(i));
    }
    return result;
}

std::vector<AlgorithmType> getAllAlgorithms() {
    std::vector<AlgorithmType> result;
    for (int i = 0; i < static_cast<int>(AlgorithmType::SENTINEL); ++i) {
        result.push_back(static_cast<AlgorithmType>(i));
    }
    return result;
}

SolveResult solve(const std::string& filename, AlgorithmType algorithm, HeuristicType heuristic) {
    Board board = loadBoard(filename);
    return solve(board, algorithm, heuristic);
}

SolveResult solve(const Board& board, AlgorithmType algorithm, HeuristicType heuristic) {
    std::unique_ptr<Solver> solver = createSolver(algorithm, board, heuristic);
    Result result = solver->solve();

    return {
        algorithm,
        heuristic,
        result.found,
        result.totalCost,
        movesToString(result.movesSolution),
        result.time,
        result.iterations,
        result.pathSolution
    };
}

void saveSolution(const std::string& inputPath, const std::string& outputPath, AlgorithmType algorithm, HeuristicType heuristic) {
    Board board = loadBoard(inputPath);
    saveSolution(board, outputPath, algorithm, heuristic);
}

void saveSolution(const Board& board, const std::string& outputPath, AlgorithmType algorithm, HeuristicType heuristic) {
    std::unique_ptr<Solver> solver = createSolver(algorithm, board, heuristic);
    Result result = solver->solve();

    solver->saveSolution(outputPath, result);
}

void playSolution(const std::string& inputPath, const SolveResult& result) {
    Board board = loadBoard(inputPath);
    playSolution(board, result);
}

void playSolution(const Board& board, const SolveResult& result) {
    std::unique_ptr<Solver> solver = createSolver(result.algorithm, board, result.heuristic);

    solver->playSolution(board, result.pathSolution);
}

void showSolutionAt(const std::string& inputPath, const SolveResult& result, int iteration) {
    Board board = loadBoard(inputPath);
    showSolutionAt(board, result, iteration);
}

void showSolutionAt(const Board& board, const SolveResult& result, int iteration) {
    std::unique_ptr<Solver> solver = createSolver(result.algorithm, board, result.heuristic);

    solver->showSolutionAt(board, result.pathSolution, iteration);
}
