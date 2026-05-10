#include "view/scenes/AlgoPlay.hpp"
#include "library/raygui.h"
#include "utils/GuiHelper.hpp"
#include <algorithm>
#include <cctype>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>

namespace
{
    const char* algorithmLabel(AlgorithmType algorithm)
    {
        switch (algorithm)
        {
            case AlgorithmType::ASTAR: return "A*";
            case AlgorithmType::UCS: return "UCS";
            case AlgorithmType::GBFS: return "GBFS";
            case AlgorithmType::DFS: return "DFS";
            case AlgorithmType::BFS: return "BFS";
            case AlgorithmType::SENTINEL: break;
        }

        return "?";
    }

    const char* algorithmFileLabel(AlgorithmType algorithm)
    {
        switch (algorithm)
        {
            case AlgorithmType::ASTAR: return "ASTAR";
            case AlgorithmType::UCS: return "UCS";
            case AlgorithmType::GBFS: return "GBFS";
            case AlgorithmType::DFS: return "DFS";
            case AlgorithmType::BFS: return "BFS";
            case AlgorithmType::SENTINEL: break;
        }

        return "UNKNOWN";
    }

    const char* heuristicLabel(HeuristicType heuristic)
    {
        switch (heuristic)
        {
            case HeuristicType::EUCLIDEAN_MIN: return "Euc Min";
            case HeuristicType::EUCLIDEAN_CHECKPOINT: return "Euc CP";
            case HeuristicType::MANHATTAN_MIN: return "Man Min";
            case HeuristicType::MANHATTAN_CHECKPOINT: return "Man CP";
            case HeuristicType::SENTINEL: break;
        }

        return "?";
    }

    bool usesHeuristic(AlgorithmType algorithm)
    {
        return algorithm == AlgorithmType::ASTAR || algorithm == AlgorithmType::GBFS;
    }

    const char* heuristicFileLabel(AlgorithmType algorithm, HeuristicType heuristic)
    {
        if (!usesHeuristic(algorithm))
        {
            return "NoHeuristic";
        }

        switch (heuristic)
        {
            case HeuristicType::EUCLIDEAN_MIN: return "EucMin";
            case HeuristicType::EUCLIDEAN_CHECKPOINT: return "EucCP";
            case HeuristicType::MANHATTAN_MIN: return "ManMin";
            case HeuristicType::MANHATTAN_CHECKPOINT: return "ManCP";
            case HeuristicType::SENTINEL: break;
        }

        return "UnknownHeuristic";
    }

    int algorithmToIndex(AlgorithmType algorithm)
    {
        switch (algorithm)
        {
            case AlgorithmType::ASTAR: return 0;
            case AlgorithmType::UCS: return 1;
            case AlgorithmType::GBFS: return 2;
            case AlgorithmType::BFS: return 3;
            case AlgorithmType::DFS: return 4;
            case AlgorithmType::SENTINEL: break;
        }

        return 0;
    }

    AlgorithmType algorithmFromIndex(int index)
    {
        switch (index)
        {
            case 0: return AlgorithmType::ASTAR;
            case 1: return AlgorithmType::UCS;
            case 2: return AlgorithmType::GBFS;
            case 3: return AlgorithmType::BFS;
            case 4: return AlgorithmType::DFS;
            default: return AlgorithmType::ASTAR;
        }
    }

    int heuristicToIndex(HeuristicType heuristic)
    {
        switch (heuristic)
        {
            case HeuristicType::MANHATTAN_CHECKPOINT: return 0;
            case HeuristicType::MANHATTAN_MIN: return 1;
            case HeuristicType::EUCLIDEAN_CHECKPOINT: return 2;
            case HeuristicType::EUCLIDEAN_MIN: return 3;
            case HeuristicType::SENTINEL: break;
        }

        return 0;
    }

    HeuristicType heuristicFromIndex(int index)
    {
        switch (index)
        {
            case 0: return HeuristicType::MANHATTAN_CHECKPOINT;
            case 1: return HeuristicType::MANHATTAN_MIN;
            case 2: return HeuristicType::EUCLIDEAN_CHECKPOINT;
            case 3: return HeuristicType::EUCLIDEAN_MIN;
            default: return HeuristicType::MANHATTAN_CHECKPOINT;
        }
    }

    std::string sanitizeFilePart(const std::string& raw)
    {
        std::string output;

        for (char c : raw)
        {
            unsigned char uc = static_cast<unsigned char>(c);
            if (std::isalnum(uc) || c == '_' || c == '-')
            {
                output.push_back(c);
            }
            else if (std::isspace(uc))
            {
                output.push_back('_');
            }
        }

        return output.empty() ? "board" : output;
    }
}

AlgoPlay::AlgoPlay(GameState &gs): 
    gameState(gs),
    renderer(gs.getPlayerType()),
    playerMotion(15.0f),
    requestedScene(SceneType::AlgoPlay),
    selectedAlgorithm(AlgorithmType::ASTAR),
    selectedHeuristic(HeuristicType::MANHATTAN_CHECKPOINT),
    algorithmDropdownEditMode(false),
    heuristicDropdownEditMode(false),
    currentStep(0),
    playbackTimer(0.0f),
    playing(false),
    statusMessage("Choose algorithm, then Run") {
    playerMotion.reset(currentPlayerPosition());
}

Point AlgoPlay::currentPlayerPosition() const
{
    if (gameState.isHasResult())
    {
        SolveResult result = gameState.getResult();
        if (!result.pathSolution.empty())
        {
            int clamped = std::max(0, std::min(currentStep, static_cast<int>(result.pathSolution.size()) - 1));
            return result.pathSolution[clamped].position;
        }
    }
    return gameState.isBoardSelected() ? gameState.getBoardRef().getStartPosition() : Point{-1, -1};
}

int AlgoPlay::currentTargetIndex() const
{
    if (!gameState.isHasResult())
    {
        return 0;
    }

    SolveResult result = gameState.getResult();
    if (result.pathSolution.empty())
    {
        return 0;
    }

    int clamped = std::max(0, std::min(currentStep, static_cast<int>(result.pathSolution.size()) - 1));
    return result.pathSolution[clamped].targetIndex;
}

void AlgoPlay::moveToStep(int nextStep)
{
    if (!gameState.isHasResult() || playerMotion.isActive()) {
        return;
    }

    SolveResult result = gameState.getResult();
    if (result.pathSolution.empty()) {
        return;
    }

    int maxStep = static_cast<int>(result.pathSolution.size()) - 1;
    int clampedNextStep = std::max(0, std::min(nextStep, maxStep));
    if (clampedNextStep == currentStep) {
        return;
    }

    Point from = result.pathSolution[currentStep].position;
    Point to = result.pathSolution[clampedNextStep].position;
    currentStep = clampedNextStep;
    playbackTimer = 0.0f;

    if (from != to) {
        playerMotion.start(from, to);
    }
    else {
        playerMotion.reset(to);
    }
}

void AlgoPlay::runSolver()
{
    if (!gameState.isBoardSelected())
    {
        statusMessage = "No board selected";
        return;
    }

    try
    {
        SolveResult result = solve(gameState.getBoardRef(), selectedAlgorithm, selectedHeuristic);
        gameState.setResult(result);
        gameState.setIsHasResult(true);
        gameState.setIsResultGenerated(true);
        currentStep = 0;
        playbackTimer = 0.0f;
        playerMotion.reset(currentPlayerPosition());
        playing = result.isFound;
        statusMessage = result.isFound ? "Solution found" : "Solution not found";
    }
    catch (const std::exception &e)
    {
        statusMessage = e.what();
        gameState.setErrorMessage(statusMessage);
    }
}

void AlgoPlay::saveCurrentSolution()
{
    if (!gameState.isBoardSelected())
    {
        statusMessage = "No board selected";
        return;
    }

    if (!gameState.isHasResult())
    {
        statusMessage = "Run solver before saving";
        return;
    }

    try
    {
        SolveResult result = gameState.getResult();
        std::filesystem::path inputPath(gameState.getStringPath());
        std::string boardName = inputPath.stem().string();
        if (boardName.empty() || boardName == "null")
        {
            boardName = "board";
        }

        std::filesystem::path outputDir("./test/output");
        std::filesystem::create_directories(outputDir);

        std::string fileName = std::string(algorithmFileLabel(result.algorithm)) + "_" +
                               heuristicFileLabel(result.algorithm, result.heuristic) + "_" +
                               sanitizeFilePart(boardName) + ".txt";
        std::filesystem::path outputPath = outputDir / fileName;

        std::ofstream output(outputPath);
        if (!output.is_open())
        {
            statusMessage = "Failed to save solution";
            return;
        }

        const Board& board = gameState.getBoardRef();
        output << "Problem:\n";
        output << board.getRows() << ' ' << board.getCols() << '\n';

        for (int row = 0; row < board.getRows(); ++row)
        {
            for (int col = 0; col < board.getCols(); ++col)
            {
                output << board.getTile(Point{row, col});
            }
            output << '\n';
        }

        for (int row = 0; row < board.getRows(); ++row)
        {
            for (int col = 0; col < board.getCols(); ++col)
            {
                if (col > 0) output << ' ';
                output << board.getCost(Point{row, col});
            }
            output << '\n';
        }

        output << "\nSolution:\n";
        output << "Algorithm     : " << algorithmLabel(result.algorithm) << '\n';
        output << "Heuristic     : " << (usesHeuristic(result.algorithm) ? heuristicLabel(result.heuristic) : "-") << '\n';
        output << "Found         : " << (result.isFound ? "Yes" : "No") << '\n';
        output << "Move solution : " << result.moves << '\n';
        output << "Total cost    : " << result.cost << '\n';
        output << "Time          : " << result.execTime << " ms\n";
        output << "Iterations    : " << result.iterations << '\n';

        statusMessage = "Saved " + outputPath.generic_string();
    }
    catch (const std::exception& e)
    {
        statusMessage = e.what();
        gameState.setErrorMessage(statusMessage);
    }
}

void AlgoPlay::update() {
    float dt = GetFrameTime();
    playerMotion.update(dt);
    renderer.update(dt, playerMotion.isActive());

    if (!playing || !gameState.isHasResult()) {
        return;
    }

    // Wait until the current slide animation finishes before starting the next solver step.
    if (playerMotion.isActive()) {
        return;
    }

    SolveResult result = gameState.getResult();
    if (result.pathSolution.empty()) {
        playing = false;
        return;
    }

    if (currentStep + 1 >= static_cast<int>(result.pathSolution.size())) {
        playing = false;
        return;
    }

    playbackTimer += dt;
    if (playbackTimer >= 0.12f) {
        moveToStep(currentStep + 1);
    }
}

void AlgoPlay::draw()
{
    DrawText("PLAY WITH ALGORITHM", 210, 25, 30, Theme::Text);

    if (GuiButton(Rectangle{20, 20, 150, 42}, "Main Menu"))
    {
        gameState.reset();
        requestedScene = SceneType::MainMenu;
    }
    if (GuiButton(Rectangle{20, 75, 150, 42}, "Change Board"))
    {
        requestedScene = SceneType::SelectBoard;
    }
    if (GuiButton(Rectangle{20, 130, 150, 42}, "Change Player"))
    {
        requestedScene = SceneType::ChoosePlayer;
    }

    DrawText("Algorithm", 20, 198, 18, Theme::Text);
    DrawText("Heuristic", 20, 272, 18, usesHeuristic(selectedAlgorithm) ? Theme::Text : Fade(Theme::Text, 0.45f));
    if (!usesHeuristic(selectedAlgorithm))
    {
        DrawText("Only A*/GBFS", 20, 294, 13, Fade(Theme::Text, 0.55f));
    }

    const bool dropdownOpen = algorithmDropdownEditMode || heuristicDropdownEditMode;
    if (!dropdownOpen)
    {
        if (GuiButton(Rectangle{20, 345, 150, 40}, "Run"))
            runSolver();

        if (GuiButton(Rectangle{20, 410, 70, 36}, playing ? "Pause" : "Play"))
            playing = !playing;
        if (GuiButton(Rectangle{100, 410, 70, 36}, "Next") && gameState.isHasResult())
        {
            moveToStep(currentStep + 1);
        }
        if (GuiButton(Rectangle{20, 455, 70, 36}, "Prev"))
        {
            moveToStep(currentStep - 1);
        }
        if (GuiButton(Rectangle{20, 510, 150, 40}, "Save Solution"))
        {
            saveCurrentSolution();
        }
    }

    int heuristicActive = heuristicToIndex(selectedHeuristic);
    if (GuiDropdownBox(Rectangle{20, 306, 150, 36}, "Man CP;Man Min;Euc CP;Euc Min", &heuristicActive, heuristicDropdownEditMode))
    {
        heuristicDropdownEditMode = !heuristicDropdownEditMode;
        if (heuristicDropdownEditMode) algorithmDropdownEditMode = false;
    }
    selectedHeuristic = heuristicFromIndex(heuristicActive);

    int algorithmActive = algorithmToIndex(selectedAlgorithm);
    if (GuiDropdownBox(Rectangle{20, 222, 150, 36}, "A*;UCS;GBFS;BFS;DFS", &algorithmActive, algorithmDropdownEditMode))
    {
        algorithmDropdownEditMode = !algorithmDropdownEditMode;
        if (algorithmDropdownEditMode) heuristicDropdownEditMode = false;
    }
    selectedAlgorithm = algorithmFromIndex(algorithmActive);

    Rectangle boardBounds{200.0f, 80.0f, GetScreenWidth() - 230.0f, GetScreenHeight() - 150.0f};
    if (gameState.isBoardSelected())
    {
        Vector2 drawPosition = playerMotion.getCurrentTilePosition();
        renderer.drawBoardAt(gameState.getBoardRef(), boardBounds, drawPosition.x, drawPosition.y, true, playerMotion.getRotationFromUpDegrees(), currentTargetIndex());
    }
    else
    {
        DrawText("No board selected", static_cast<int>(boardBounds.x), static_cast<int>(boardBounds.y), 24, Theme::Error);
    }

    DrawText(statusMessage.c_str(), 200, GetScreenHeight() - 58, 20, Theme::Text);
    if (gameState.isHasResult())
    {
        SolveResult result = gameState.getResult();
        const char* heuristicText = usesHeuristic(result.algorithm) ? heuristicLabel(result.heuristic) : "-";
        DrawText(TextFormat("%s | H: %s | Found: %s | Cost: %d | Moves: %s", algorithmLabel(result.algorithm), heuristicText, result.isFound ? "Yes" : "No", result.cost, result.moves.c_str()),
                 200, GetScreenHeight() - 32, 18, Theme::Text);
        DrawText(TextFormat("Step: %d/%d | Time: %.3f ms | Iterations: %d", currentStep,
                            static_cast<int>(result.pathSolution.size()) > 0 ? static_cast<int>(result.pathSolution.size()) - 1 : 0,
                            result.execTime, result.iterations),
                 200, GetScreenHeight() - 12, 16, Theme::Border);
    }
}

SceneType AlgoPlay::nextScene()
{
    return requestedScene;
}
