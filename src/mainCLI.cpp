#include "solver/Algorithm/AStar.hpp"
#include "solver/Algorithm/GBFS.hpp"
#include "solver/Algorithm/UCS.hpp"
#include "utils/Loader.hpp"
#include <algorithm>
#include <cctype>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace {
    AlgorithmType parseAlgorithmType(const string& input) {
        if (input == "UCS") {
            return AlgorithmType::UCS;
        }

        if (input == "GBFS") {
            return AlgorithmType::GBFS;
        }

        if (input == "ASTAR" || input == "A*") {
            return AlgorithmType::ASTAR;
        }

        throw invalid_argument("Choose only available valid given algorithms");
    }

    HeuristicType parseHeuristicType(const string& input) {
        if (input == "Euclidean") {
            return HeuristicType::EUCLIDEAN;
        }
        if (input == "Manhattan") {
            return HeuristicType::MANHATTAN;
        }
        //more to come
        throw invalid_argument("Choose only available valid given heuristics");
    }

    string AlgorithmTypeToString(AlgorithmType algorithm) {
        switch (algorithm) {
            case AlgorithmType::UCS:
                return "UCS";
            case AlgorithmType::GBFS:
                return "GBFS";
            case AlgorithmType::ASTAR:
                return "A*";
            case AlgorithmType::BFS:
                return "BFS";
            case AlgorithmType::DFS:
                return "DFS";
        }
    }

    unique_ptr<Solver> createSolver(AlgorithmType algorithm, const Board& board, HeuristicType heuristicType) {
        switch (algorithm) {
            case AlgorithmType::ASTAR:
                return make_unique<AStar>(board, heuristicType);
            case AlgorithmType::UCS:
                throw runtime_error("belum implementasi ucs");
            case AlgorithmType::GBFS:
                throw runtime_error("beum implementasi gbfs");
            case AlgorithmType::BFS:
                throw runtime_error("belum implementasi bfs");
            case AlgorithmType::DFS:
                throw runtime_error("belum implementasi dfs");
        }

        throw runtime_error("Invalid Algorithm");
    }

    void printMoves(const vector<Direction>& moves) {
        if (moves.empty()) {
            cout << "-";
            return;
        }

        for (Direction move : moves) {
            cout << move;
        }
        cout << "\n";
    }

    bool isStringANumber(const string& input) {
        if (input.empty()) {
            return false;
        }
        for (char c : input) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }
}

int main() {
    string filename;
    string algorithmInput;
    string heuristicInput;

    Board board = Board();
    AlgorithmType algorithm;
    HeuristicType heuristic;

    while (true) {
        cout << "Masukkan file input: ";
        getline(cin, filename);

        if (filename.empty()) {
            cout << "File tidak boleh kosong.\n";
            continue;
        }

        try {
            board = loadBoard(filename);
            cout << "Board berhasil dibaca:\n";
            board.printBoard();
            break;
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }
    }

    while (true) {
        cout << "Pilih algoritma (UCS/GBFS/A*): ";
        getline(cin, algorithmInput);

        try {
            algorithm = parseAlgorithmType(algorithmInput);
            break;
        }
        catch (...) {
            cout << "Input algoritma tidak valid. Coba lagi.\n";
        }
    }

    while (true) {
        cout << "Pilih heuristic (Euclidean/Manhattan): ";
        getline(cin, heuristicInput);

        try {
            heuristic = parseHeuristicType(heuristicInput);
            break;
        }
        catch (...) {
            cout << "Input heuristic tidak valid. Coba lagi.\n";
        }
    }

    try {
        unique_ptr<Solver> solver = createSolver(algorithm, board, heuristic);

        Result result = solver->solve();

        if (!result.found) {
            cout << "\n=== Hasil " << AlgorithmTypeToString(algorithm) << " ===\n";
            cout << "Found: " << (result.found ? "Yes" : "Not found") << '\n';
            cout << "Waktu eksekusi: " << result.time << " ms\n";
            cout << "Banyak expanded nodes: " << result.expandedPaths.size() << '\n';
            return 0;
        }

        cout << "\nIlustrasi solusi:\n";
        cout << "Moves solusi: ";
        printMoves(result.movesSolution);
        cout << "\n";

        solver->playSolution(board, result.pathSolution);

        cout << "\n>>> Hasil " << AlgorithmTypeToString(algorithm) << " <<<\n";
        cout << "Found: " << (result.found ? "Yes" : "Not found") << '\n';
        cout << "Cost solusi: " << result.totalCost << '\n';
        cout << "Moves solusi: ";  
        printMoves(result.movesSolution);
        cout << "Waktu eksekusi: " << result.time<< " ms\n";
        cout << "Banyak iterasi: " << result.iterations << '\n';

        string wantsPlayback;
        while (true) {
            cout << "\nApakah ingin melakukan playback? (Y/N): ";
            getline(cin, wantsPlayback);

            if (!wantsPlayback.empty() &&
                (wantsPlayback[0] == 'Y' || wantsPlayback[0] == 'y' ||
                 wantsPlayback[0] == 'N' || wantsPlayback[0] == 'n')) {
                break;
            }

            cout << "Input harus Y atau N.\n";
        }

        while (wantsPlayback[0] == 'Y' || wantsPlayback[0] == 'y') {
            string iteration;
            int iterationNum;
            while (true) {
                cout << "\nPada iterasi ke berapa melakukan playback?: ";
                getline(cin, iteration);
                if (isStringANumber(iteration)) {
                    iterationNum = stoi(iteration);
                    if ((iterationNum >= 0 && iterationNum <= result.iterations-1)) {
                        solver->showSolutionAt(board, result.pathSolution, iterationNum);
                        break;
                    }
                }
                cout << "Input harus angka dan berada di interval 0 sampai " << result.iterations-1 << "\n";
            }

            while (true) {
                cout << "\nApakah ingin melakukan playback? (Y/N): ";
                getline(cin, wantsPlayback);

                if (!wantsPlayback.empty() &&
                    (wantsPlayback[0] == 'Y' || wantsPlayback[0] == 'y' ||
                    wantsPlayback[0] == 'N' || wantsPlayback[0] == 'n')) {
                    break;
                }

                cout << "Input harus Y atau N.\n";
            }
        }

        string saveAnswer;
        while (true) {
            cout << "\nApakah ingin menyimpan solusi? (Y/N): ";
            getline(cin, saveAnswer);

            if (!saveAnswer.empty() &&
                (saveAnswer[0] == 'Y' || saveAnswer[0] == 'y' ||
                 saveAnswer[0] == 'N' || saveAnswer[0] == 'n')) {
                break;
            }

            cout << "Input harus Y atau N.\n";
        }

        if (saveAnswer[0] == 'Y' || saveAnswer[0] == 'y') {
            string outputPath;

            while (true) {
                cout << "Masukkan file output: ";
                getline(cin, outputPath);

                if (outputPath.empty()) {
                    cout << "Path tidak boleh kosong.\n";
                    continue;
                }

                try {
                    solver->saveSolution(outputPath, result);
                    cout << "Solusi disimpan pada " << outputPath << '\n';
                    break;
                }
                catch (const exception& e) {
                    cout << "Gagal menyimpan: " << e.what() << '\n';
                }
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
