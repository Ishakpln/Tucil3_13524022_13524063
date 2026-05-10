#include "view/scenes/SelectBoard.hpp"
#include "library/raygui.h"
#include "utils/Loader.hpp"
#include <algorithm>
#include <filesystem>

namespace {
    std::string displayName(const std::string& path) {
        return std::filesystem::path(path).filename().string();
    }
}

SelectBoard::SelectBoard(GameState& gs):
    gameState(gs),
    renderer(gs.getPlayerType()),
    selectedIndex(0),
    previewBoard(),
    hasPreview(false),
    requestedScene(SceneType::SelectBoard),
    errorMessage("")
{
    discoverBoards();
    loadPreview();
}

void SelectBoard::discoverBoards() {
    boardPaths.clear();

    try {
        const std::filesystem::path testDir("./test");
        if (std::filesystem::exists(testDir)) {
            for (const auto& entry : std::filesystem::directory_iterator(testDir)) {
                if (entry.is_regular_file()) {
                    boardPaths.push_back(entry.path().generic_string());
                }
            }
        }
    }
    catch (...) {
        // Fall back to known repository sample below.
    }

    std::sort(boardPaths.begin(), boardPaths.end());
    if (boardPaths.empty()) {
        boardPaths.push_back("./test/1.txt");
    }
}

void SelectBoard::loadPreview() {
    hasPreview = false;
    errorMessage.clear();

    if (boardPaths.empty()) {
        errorMessage = "No board file found in ./test";
        return;
    }

    try {
        previewBoard = loadBoard(boardPaths[selectedIndex]);
        hasPreview = true;
    }
    catch (const std::exception& e) {
        errorMessage = e.what();
    }
}

SceneType SelectBoard::playgroundScene() const {
    return gameState.getPlayMode() == PlayMode::ALGORITHM ? SceneType::AlgoPlay : SceneType::ManualPlay;
}

void SelectBoard::update() {
    renderer.update(GetFrameTime(), true);
}

void SelectBoard::draw() {
    DrawText("SELECT BOARD", 40, 30, 36, BLACK);

    if (GuiButton(Rectangle{20, 20, 150, 45}, "Main Menu")) {
        gameState.reset();
        requestedScene = SceneType::MainMenu;
    }

    const float listX = 40.0f;
    const float listY = 100.0f;
    const float buttonW = 220.0f;
    const float buttonH = 42.0f;

    for (int i = 0; i < static_cast<int>(boardPaths.size()) && i < 8; ++i) {
        const std::string label = (i == selectedIndex ? "> " : "  ") + displayName(boardPaths[i]);
        if (GuiButton(Rectangle{listX, listY + i * (buttonH + 8.0f), buttonW, buttonH}, label.c_str())) {
            selectedIndex = i;
            loadPreview();
        }
    }

    if (GuiButton(Rectangle{listX, GetScreenHeight() - 125.0f, buttonW, 45.0f}, "CREATE BOARD")) {
        requestedScene = SceneType::BoardEditor;
    }

    if (GuiButton(Rectangle{listX, GetScreenHeight() - 70.0f, buttonW, 45.0f}, "OK")) {
        if (hasPreview) {
            gameState.setBoard(previewBoard);
            gameState.setStringPath(boardPaths[selectedIndex]);
            gameState.setBoardSelected(true);
            gameState.setGameOver(false);
            gameState.setWinning(false);
            gameState.clearResult();
            requestedScene = playgroundScene();
        }
    }

    Rectangle previewBounds{300.0f, 90.0f, GetScreenWidth() - 330.0f, GetScreenHeight() - 140.0f};
    DrawRectangleLinesEx(previewBounds, 2.0f, Fade(BLACK, 0.3f));
    DrawText("Preview Board", static_cast<int>(previewBounds.x), static_cast<int>(previewBounds.y - 30), 22, BLACK);

    if (hasPreview) {
        renderer.drawBoard(previewBoard, previewBounds, previewBoard.getStartPosition(), true);
    }
    else {
        DrawText(errorMessage.c_str(), static_cast<int>(previewBounds.x + 20), static_cast<int>(previewBounds.y + 20), 20, RED);
    }
}

SceneType SelectBoard::nextScene() {
    return requestedScene;
}
