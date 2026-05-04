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

        throw invalid_argument("Choose only available valid given algorithms");
    }

    HeuristicType parseHeuristicType(const string& input) {
        if (input == "Euclidean_min") {
            return HeuristicType::EUCLIDEAN_MIN;
        }
        if (input == "Euclidean_avg") {
            return HeuristicType::EUCLIDEAN_AVG;
        }
        if (input == "Manhattan_min") {
            return HeuristicType::MANHATTAN_MIN;
        }
        if (input == "Manhattan_avg") {
            return HeuristicType::MANHATTAN_AVG;
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
            case AlgorithmType::SENTINEL:
                break;
        }

        return "Unknown";
    }   

    string HeuristicTypeToString(HeuristicType heuristic) {
        switch (heuristic) {
            case HeuristicType::EUCLIDEAN_MIN:
                return "Euclidean_min";
            case HeuristicType::EUCLIDEAN_AVG:
                return "Euclidean_avg";
            case HeuristicType::MANHATTAN_MIN:
                return "Manhattan_min";
            case HeuristicType::MANHATTAN_AVG:
                return "Manhattan_avg";
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
        cout << "Pilih heuristic (Euclidean_min/Euclidean_avg/Manhattan_min/Manhattan_avg): ";
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
        SolveResult result = solve(board, algorithm, heuristic);

        if (!result.isFound) {
            cout << "\n=== Hasil " << AlgorithmTypeToString(result.algorithm) << " ===\n";
            cout << "Heuristic: " << HeuristicTypeToString(result.heuristic) << '\n';
            cout << "Found: Not found\n";
            cout << "Waktu eksekusi: " << result.execTime << " ms\n";
            cout << "Banyak iterasi: " << result.iterations << '\n';
            return 0;
        }

        cout << "\nIlustrasi solusi:\n";
        playSolution(board, result);

        cout << "\n>>> Hasil " << AlgorithmTypeToString(result.algorithm) << " <<<\n";
        cout << "Heuristic: " << HeuristicTypeToString(result.heuristic) << '\n';
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
