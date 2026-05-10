#include "view/scenes/AlgoPlay.hpp"
#include "library/raygui.h"
#include "utils/GuiHelper.hpp"
#include <algorithm>
#include <exception>

AlgoPlay::AlgoPlay(GameState &gs): 
    gameState(gs),
    renderer(gs.getPlayerType()),
    requestedScene(SceneType::AlgoPlay),
    selectedAlgorithm(AlgorithmType::ASTAR),
    selectedHeuristic(HeuristicType::MANHATTAN_CHECKPOINT),
    currentStep(0),
    playbackTimer(0.0f),
    playing(false),
    statusMessage("Choose algorithm, then Run") {}

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
        playing = result.isFound;
        statusMessage = result.isFound ? "Solution found" : "Solution not found";
    }
    catch (const std::exception &e)
    {
        statusMessage = e.what();
        gameState.setErrorMessage(statusMessage);
    }
}

void AlgoPlay::update() {
    float dt = GetFrameTime();
    bool movedThisFrame = false;

    if (playing && gameState.isHasResult()) {
        SolveResult result = gameState.getResult();
        playbackTimer += dt;

        if (playbackTimer >= 0.35f) {
            playbackTimer = 0.0f;

            if (currentStep + 1 < static_cast<int>(result.pathSolution.size())) {
                Point oldPosition = result.pathSolution[currentStep].position;
                ++currentStep;
                Point newPosition = result.pathSolution[currentStep].position;

                movedThisFrame = oldPosition.x != newPosition.x || oldPosition.y != newPosition.y;
            }
            else {
                playing = false;
            }
        }
    }

    renderer.update(dt, movedThisFrame);
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

    if (GuiButton(Rectangle{20, 200, 70, 36}, selectedAlgorithm == AlgorithmType::ASTAR ? ">A*" : "A*"))
        selectedAlgorithm = AlgorithmType::ASTAR;
    if (GuiButton(Rectangle{100, 200, 70, 36}, selectedAlgorithm == AlgorithmType::UCS ? ">UCS" : "UCS"))
        selectedAlgorithm = AlgorithmType::UCS;
    if (GuiButton(Rectangle{20, 245, 70, 36}, selectedAlgorithm == AlgorithmType::GBFS ? ">GBFS" : "GBFS"))
        selectedAlgorithm = AlgorithmType::GBFS;
    if (GuiButton(Rectangle{100, 245, 70, 36}, "Run"))
        runSolver();

    if (GuiButton(Rectangle{20, 300, 70, 36}, playing ? "Pause" : "Play"))
        playing = !playing;
    if (GuiButton(Rectangle{100, 300, 70, 36}, "Next") && gameState.isHasResult())
    {
        SolveResult result = gameState.getResult();
        if (!result.pathSolution.empty())
        {
            currentStep = std::min(currentStep + 1, static_cast<int>(result.pathSolution.size()) - 1);
        }
    }
    if (GuiButton(Rectangle{20, 345, 70, 36}, "Prev"))
    {
        currentStep = std::max(0, currentStep - 1);
    }

    Rectangle boardBounds{200.0f, 80.0f, GetScreenWidth() - 230.0f, GetScreenHeight() - 150.0f};
    if (gameState.isBoardSelected())
    {
        renderer.drawBoard(gameState.getBoardRef(), boardBounds, currentPlayerPosition(), true);
    }
    else
    {
        DrawText("No board selected", static_cast<int>(boardBounds.x), static_cast<int>(boardBounds.y), 24, Theme::Error);
    }

    DrawText(statusMessage.c_str(), 200, GetScreenHeight() - 58, 20, Theme::Text);
    if (gameState.isHasResult())
    {
        SolveResult result = gameState.getResult();
        DrawText(TextFormat("Found: %s | Cost: %d | Moves: %s", result.isFound ? "Yes" : "No", result.cost, result.moves.c_str()),
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
