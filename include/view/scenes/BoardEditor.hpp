#pragma once

#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/assets/BoardRenderer.hpp"
#include "raylib.h"

#include <string>
#include <vector>

class BoardEditor : public Scene
{
private:
    GameState& gameState;
    BoardRenderer renderer;
    SceneType requestedScene;
    Vector2 editorScroll;

    int rows;
    int cols;
    int draftRows;
    int draftCols;
    bool rowValueEditMode;
    bool colValueEditMode;

    std::vector<char> tiles;
    std::vector<int> costs;

    int selectedRow;
    int selectedCol;
    char selectedBrush;
    bool costValueEditMode;

    std::vector<std::string> boardPaths;
    int selectedBoardIndex;

    bool showSavePopup;
    bool boardNameEditMode;
    char boardName[64];

    std::string statusMessage;
    std::string errorMessage;

    int indexOf(int row, int col) const;
    char tileAt(int row, int col) const;
    int costAt(int row, int col) const;

    void createDefaultBoard(int newRows, int newCols);
    void resizeBoard(int newRows, int newCols);
    void discoverBoards();
    void loadBoardIntoEditor(const std::string& path);

    void applyBrush(int row, int col);
    void selectCell(int row, int col);
    void normalizeCosts();

    Rectangle getBoardBounds(Rectangle area) const;
    Rectangle getCostBounds(Rectangle area) const;
    bool cellFromMouse(Rectangle gridBounds, int& row, int& col) const;

    void drawLeftPanel(Rectangle panel);
    void drawBoardGrid(Rectangle bounds);
    void drawCostGrid(Rectangle bounds);
    void drawSavePopup();

    bool validateBoard(std::string& message) const;
    std::string sanitizedBoardName() const;
    bool saveCurrentBoard(std::string& message);

public:
    BoardEditor(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};
