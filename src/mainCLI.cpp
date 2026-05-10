#include "utils/Bridge.hpp"
#include "utils/Loader.hpp"
#include <cctype>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

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

        if (input == "BFS") {
            return AlgorithmType::BFS;
        }

        if (input == "DFS") {
            return AlgorithmType::DFS;
        }

        throw invalid_argument("Choose only available valid given algorithms");
    }

    HeuristicType parseHeuristicType(const string& input) {
        if (input == "Euclidean_min") {
            return HeuristicType::EUCLIDEAN_MIN;
        }
        if (input == "Euclidean_checkpoint") {
            return HeuristicType::EUCLIDEAN_CHECKPOINT;
        }
        if (input == "Manhattan_min") {
            return HeuristicType::MANHATTAN_MIN;
        }
        if (input == "Manhattan_checkpoint") {
            return HeuristicType::MANHATTAN_CHECKPOINT;
        }
        //more to come
        throw invalid_argument("Choose only available valid given heuristics");
    }

    bool usesHeuristic(AlgorithmType algorithm) {
        return algorithm == AlgorithmType::ASTAR ||
               algorithm == AlgorithmType::GBFS;
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
            case AlgorithmType::SENTINEL:
                break;
        }

        return "Unknown";
    }   

    string HeuristicTypeToString(HeuristicType heuristic) {
        switch (heuristic) {
            case HeuristicType::EUCLIDEAN_MIN:
                return "Euclidean_min";
            case HeuristicType::EUCLIDEAN_CHECKPOINT:
                return "Euclidean_checkpoint";
            case HeuristicType::MANHATTAN_MIN:
                return "Manhattan_min";
            case HeuristicType::MANHATTAN_CHECKPOINT:
                return "Manhattan_checkpoint";
            case HeuristicType::SENTINEL:
                break;
        }

        return "Unknown";
    }

    bool isStringANumber(const string& input) {
        if (input.empty()) {
            return false;
        }

        for (char c : input) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                return false;
            }
        }

        return true;
    }

    bool readYesNo(const string& prompt) {
        string answer;

        while (true) {
            cout << prompt;
            getline(cin, answer);

            if (!answer.empty()) {
                if (answer[0] == 'Y' || answer[0] == 'y') {
                    return true;
                }

                if (answer[0] == 'N' || answer[0] == 'n') {
                    return false;
                }
            }

            cout << "Input harus Y atau N.\n";
        }
    }

}

int main() {
    string filename;
    string algorithmInput;
    string heuristicInput;

    Board board = Board();
    AlgorithmType algorithm;
    HeuristicType heuristic = HeuristicType::SENTINEL;

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
        cout << "Pilih algoritma (UCS/GBFS/A*/BFS/DFS): ";
        getline(cin, algorithmInput);

        try {
            algorithm = parseAlgorithmType(algorithmInput);
            break;
        }
        catch (...) {
            cout << "Input algoritma tidak valid. Coba lagi.\n";
        }
    }

    if (usesHeuristic(algorithm)) {
        while (true) {
            cout << "Pilih heuristic (Euclidean_min/Euclidean_checkpoint/Manhattan_min/Manhattan_checkpoint): ";
            getline(cin, heuristicInput);

            try {
                heuristic = parseHeuristicType(heuristicInput);
                break;
            }
            catch (...) {
                cout << "Input heuristic tidak valid. Coba lagi.\n";
            }
        }
    }

    try {
        SolveResult result = solve(board, algorithm, heuristic);

        if (!result.isFound) {
            cout << "\n=== Hasil " << AlgorithmTypeToString(result.algorithm) << " ===\n";
            if (usesHeuristic(result.algorithm)) {
                cout << "Heuristic: " << HeuristicTypeToString(result.heuristic) << '\n';
            }
            cout << "Found: Not found\n";
            cout << "Waktu eksekusi: " << result.execTime << " ms\n";
            cout << "Banyak iterasi: " << result.iterations << '\n';
            return 0;
        }

        cout << "\nIlustrasi solusi:\n";
        playSolution(board, result);

        cout << "\n>>> Hasil " << AlgorithmTypeToString(result.algorithm) << " <<<\n";
        if (usesHeuristic(result.algorithm)) {
            cout << "Heuristic: " << HeuristicTypeToString(result.heuristic) << '\n';
        }
        cout << "Found: Yes\n";
        cout << "Cost solusi: " << result.cost << '\n';
        cout << "Moves solusi: " << result.moves << '\n';
        cout << "Waktu eksekusi: " << result.execTime << " ms\n";
        cout << "Banyak iterasi: " << result.iterations << '\n';

        while (readYesNo("\nApakah ingin melakukan playback? (Y/N): ")) {
            string iteration;
            int iterationNum;

            while (true) {
                cout << "\nPada iterasi ke berapa melakukan playback?: ";
                getline(cin, iteration);

                if (isStringANumber(iteration)) {
                    iterationNum = stoi(iteration);

                    if (iterationNum >= 0 && iterationNum < static_cast<int>(result.pathSolution.size())) {
                        showSolutionAt(board, result, iterationNum);
                        break;
                    }
                }

                cout << "Input harus angka dan berada di interval 0 sampai "
                     << static_cast<int>(result.pathSolution.size()) - 1 << "\n";
            }
        }

        if (readYesNo("\nApakah ingin menyimpan solusi? (Y/N): ")) {
            string outputPath;

            while (true) {
                cout << "Masukkan file output: ";
                getline(cin, outputPath);

                if (outputPath.empty()) {
                    cout << "Path tidak boleh kosong.\n";
                    continue;
                }

                try {
                    saveSolution(board, outputPath, result.algorithm, result.heuristic);
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
