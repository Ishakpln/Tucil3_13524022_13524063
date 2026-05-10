#include "view/scenes/BoardEditor.hpp"

#include "library/raygui.h"
#include "utils/GuiHelper.hpp"
#include "utils/Loader.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace
{
    constexpr int kMinRows = 3;
    constexpr int kMinCols = 3;
    constexpr int kMaxRows = 20;
    constexpr int kMaxCols = 20;

    std::string displayName(const std::string& path)
    {
        return std::filesystem::path(path).filename().string();
    }

    bool isCheckpoint(char c)
    {
        return c >= '0' && c <= '9';
    }

    bool isValidEditorTile(char c)
    {
        return c == 'X' || c == '*' || c == 'L' || c == 'Z' || c == 'O' || isCheckpoint(c);
    }

    std::string trimExtension(std::string name)
    {
        if (name.size() >= 4 && name.substr(name.size() - 4) == ".txt")
        {
            name.resize(name.size() - 4);
        }
        return name;
    }

    void drawCenteredText(const char* text, Rectangle bounds, int fontSize, Color color)
    {
        const int textWidth = MeasureText(text, fontSize);
        DrawText(
            text,
            static_cast<int>(bounds.x + (bounds.width - textWidth) / 2.0f),
            static_cast<int>(bounds.y + (bounds.height - fontSize) / 2.0f),
            fontSize,
            color
        );
    }
}

BoardEditor::BoardEditor(GameState &gs) :
    gameState(gs),
    renderer(gs.getPlayerType()),
    requestedScene(SceneType::BoardEditor),
    editorScroll{0.0f, 0.0f},
    rows(7),
    cols(7),
    draftRows(7),
    draftCols(7),
    rowValueEditMode(false),
    colValueEditMode(false),
    tiles(),
    costs(),
    selectedRow(1),
    selectedCol(1),
    selectedBrush('*'),
    costValueEditMode(false),
    costInputFresh(false),
    boardPaths(),
    selectedBoardIndex(0),
    showSavePopup(false),
    boardNameEditMode(false),
    boardName{0},
    statusMessage("Click a component, then click the board to paint."),
    errorMessage("")
{
    std::snprintf(boardName, sizeof(boardName), "custom_board");
    createDefaultBoard(rows, cols);
    discoverBoards();
}

int BoardEditor::indexOf(int row, int col) const
{
    return row * cols + col;
}

char BoardEditor::tileAt(int row, int col) const
{
    return tiles[indexOf(row, col)];
}

int BoardEditor::costAt(int row, int col) const
{
    return costs[indexOf(row, col)];
}

void BoardEditor::createDefaultBoard(int newRows, int newCols)
{
    rows = std::clamp(newRows, kMinRows, kMaxRows);
    cols = std::clamp(newCols, kMinCols, kMaxCols);
    draftRows = rows;
    draftCols = cols;

    tiles.assign(rows * cols, '*');
    costs.assign(rows * cols, 1);

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            const bool border = row == 0 || col == 0 || row == rows - 1 || col == cols - 1;
            if (border)
            {
                tiles[indexOf(row, col)] = 'X';
                costs[indexOf(row, col)] = 999;
            }
        }
    }

    tiles[indexOf(rows - 2, 1)] = 'Z';
    tiles[indexOf(1, cols - 2)] = 'O';
    costs[indexOf(rows - 2, 1)] = 1;
    costs[indexOf(1, cols - 2)] = 1;

    selectedRow = std::clamp(selectedRow, 0, rows - 1);
    selectedCol = std::clamp(selectedCol, 0, cols - 1);
    costValueEditMode = false;
    costInputFresh = false;
}

void BoardEditor::resizeBoard(int newRows, int newCols)
{
    newRows = std::clamp(newRows, kMinRows, kMaxRows);
    newCols = std::clamp(newCols, kMinCols, kMaxCols);

    std::vector<char> oldTiles = tiles;
    std::vector<int> oldCosts = costs;
    const int oldRows = rows;
    const int oldCols = cols;

    rows = newRows;
    cols = newCols;
    draftRows = rows;
    draftCols = cols;
    tiles.assign(rows * cols, '*');
    costs.assign(rows * cols, 1);

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            const int newIndex = indexOf(row, col);
            if (row < oldRows && col < oldCols)
            {
                tiles[newIndex] = oldTiles[row * oldCols + col];
                costs[newIndex] = oldCosts[row * oldCols + col];
            }

            if (tiles[newIndex] == 'X') costs[newIndex] = 999;
        }
    }

    selectedRow = std::clamp(selectedRow, 0, rows - 1);
    selectedCol = std::clamp(selectedCol, 0, cols - 1);
    costValueEditMode = false;
    costInputFresh = false;
    normalizeCosts();
    statusMessage = "Board size updated.";
}

void BoardEditor::discoverBoards()
{
    boardPaths.clear();

    try
    {
        const std::filesystem::path testDir("./test");
        if (std::filesystem::exists(testDir))
        {
            for (const auto& entry : std::filesystem::directory_iterator(testDir))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".txt")
                {
                    boardPaths.push_back(entry.path().generic_string());
                }
            }
        }
    }
    catch (...)
    {
        // Keep editor usable even if discovery fails.
    }

    std::sort(boardPaths.begin(), boardPaths.end());
    if (selectedBoardIndex >= static_cast<int>(boardPaths.size())) selectedBoardIndex = 0;
}

void BoardEditor::loadBoardIntoEditor(const std::string& path)
{
    try
    {
        Board board = loadBoard(path);
        rows = board.getRows();
        cols = board.getCols();
        draftRows = rows;
        draftCols = cols;
        tiles.assign(rows * cols, '*');
        costs.assign(rows * cols, 1);

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                Point p{row, col};
                tiles[indexOf(row, col)] = board.getTile(p);
                costs[indexOf(row, col)] = board.getCost(p);
            }
        }

        selectedRow = std::clamp(selectedRow, 0, rows - 1);
        selectedCol = std::clamp(selectedCol, 0, cols - 1);
        costValueEditMode = false;
        costInputFresh = false;
        std::string name = trimExtension(displayName(path));
        std::snprintf(boardName, sizeof(boardName), "%s", name.c_str());
        normalizeCosts();
        statusMessage = "Loaded " + displayName(path) + ".";
        errorMessage.clear();
    }
    catch (const std::exception& e)
    {
        errorMessage = e.what();
    }
}

void BoardEditor::applyBrush(int row, int col)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols) return;
    if (!isValidEditorTile(selectedBrush)) return;

    if (selectedBrush == 'Z' || selectedBrush == 'O' || isCheckpoint(selectedBrush))
    {
        for (char& tile : tiles)
        {
            if (tile == selectedBrush) tile = '*';
        }
    }

    selectCell(row, col);
    tiles[indexOf(row, col)] = selectedBrush;

    if (selectedBrush == 'X')
    {
        costs[indexOf(row, col)] = 999;
    }
    else if (costs[indexOf(row, col)] == 999)
    {
        costs[indexOf(row, col)] = 1;
    }

    normalizeCosts();
}

void BoardEditor::selectCell(int row, int col)
{
    selectedRow = std::clamp(row, 0, rows - 1);
    selectedCol = std::clamp(col, 0, cols - 1);
}

void BoardEditor::normalizeCosts()
{
    for (int i = 0; i < static_cast<int>(tiles.size()); ++i)
    {
        if (tiles[i] == 'X')
        {
            costs[i] = 999;
        }
        else if (costs[i] < 0)
        {
            costs[i] = 0;
        }
    }
}

Rectangle BoardEditor::getBoardBounds(Rectangle area) const
{
    const float cellSize = std::min(area.width / static_cast<float>(cols), area.height / static_cast<float>(rows));
    const float width = cellSize * cols;
    const float height = cellSize * rows;
    return Rectangle{area.x + (area.width - width) / 2.0f, area.y + (area.height - height) / 2.0f, width, height};
}

Rectangle BoardEditor::getCostBounds(Rectangle area) const
{
    return getBoardBounds(area);
}

bool BoardEditor::cellFromMouse(Rectangle gridBounds, int& row, int& col) const
{
    Vector2 mouse = GetMousePosition();
    if (!CheckCollisionPointRec(mouse, gridBounds)) return false;

    const float cellW = gridBounds.width / static_cast<float>(cols);
    const float cellH = gridBounds.height / static_cast<float>(rows);
    col = static_cast<int>((mouse.x - gridBounds.x) / cellW);
    row = static_cast<int>((mouse.y - gridBounds.y) / cellH);
    row = std::clamp(row, 0, rows - 1);
    col = std::clamp(col, 0, cols - 1);
    return true;
}

void BoardEditor::drawLeftPanel(Rectangle panel)
{
    const float x = panel.x;
    const float y = panel.y;
    const float w = panel.width;

    DrawRectangleRec(panel, Theme::Surface);
    DrawRectangleLinesEx(panel, 2.0f, Theme::Border);

    DrawText("COMPONENTS", static_cast<int>(x + 18), static_cast<int>(y + 14), 17, Theme::Text);
    DrawText("Select, then click board", static_cast<int>(x + 18), static_cast<int>(y + 36), 13, Theme::Text);

    const std::array<char, 15> brushes = {'X', '*', 'L', 'Z', 'O', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    const float startX = x + 18.0f;
    const float startY = y + 68.0f;
    const float size = 36.0f;
    const float gap = 7.0f;

    for (int i = 0; i < static_cast<int>(brushes.size()); ++i)
    {
        const int column = i % 5;
        const int row = i / 5;

        Rectangle b{
            startX + column * (size + gap),
            startY + row * (size + gap),
            size,
            size
        };

        char label[2] = {brushes[i], '\0'};

        if (brushes[i] == selectedBrush)
        {
            DrawRectangleRec(
                Rectangle{b.x - 3.0f, b.y - 3.0f, b.width + 6.0f, b.height + 6.0f},
                Theme::AccentDark
            );
        }

        if (GuiButton(b, label))
        {
            selectedBrush = brushes[i];
            costValueEditMode = false;
            costInputFresh = false;
        }
    }

    DrawText(TextFormat("Brush: %c", selectedBrush), static_cast<int>(x + 18), static_cast<int>(y + 215), 16, Theme::Text);

    DrawText("Legend:", static_cast<int>(x + 18), static_cast<int>(y + 244), 15, Theme::Text);
    DrawText("X obstacle, * path", static_cast<int>(x + 18), static_cast<int>(y + 265), 13, Theme::Text);
    DrawText("L lava, Z start", static_cast<int>(x + 18), static_cast<int>(y + 283), 13, Theme::Text);
    DrawText("O goal, 0-9 checkpoints", static_cast<int>(x + 18), static_cast<int>(y + 301), 13, Theme::Text);

    DrawText("SIZE", static_cast<int>(x + 18), static_cast<int>(y + 334), 16, Theme::Text);

    if (GuiValueBox(Rectangle{x + 18.0f, y + 360.0f, 80.0f, 32.0f}, "Rows", &draftRows, kMinRows, kMaxRows, rowValueEditMode))
    {
        rowValueEditMode = !rowValueEditMode;
    }

    if (GuiValueBox(Rectangle{x + 122.0f, y + 360.0f, 80.0f, 32.0f}, "Cols", &draftCols, kMinCols, kMaxCols, colValueEditMode))
    {
        colValueEditMode = !colValueEditMode;
    }

    if (GuiButton(Rectangle{x + 18.0f, y + 402.0f, w - 36.0f, 34.0f}, "APPLY SIZE"))
    {
        resizeBoard(draftRows, draftCols);
    }

    DrawText("BOARD DATA", static_cast<int>(x + 18), static_cast<int>(y + 458), 16, Theme::Text);

    if (boardPaths.empty())
    {
        DrawText("No board files", static_cast<int>(x + 18), static_cast<int>(y + 484), 13, Theme::Error);
    }
    else
    {
        for (int i = 0; i < static_cast<int>(boardPaths.size()) && i < 3; ++i)
        {
            Rectangle b{x + 18.0f, y + 484.0f + i * 35.0f, w - 36.0f, 30.0f};
            std::string label = (i == selectedBoardIndex ? "> " : "  ") + displayName(boardPaths[i]);

            if (GuiButton(b, label.c_str()))
            {
                selectedBoardIndex = i;
                loadBoardIntoEditor(boardPaths[i]);
            }
        }
    }

    const float bottomButtonY = panel.y + panel.height - 54.0f;

    if (GuiButton(Rectangle{x + 18.0f, bottomButtonY, 85.0f, 34.0f}, "NEW"))
    {
        createDefaultBoard(draftRows, draftCols);
        statusMessage = "New board created.";
    }

    if (GuiButton(Rectangle{x + w - 103.0f, bottomButtonY, 85.0f, 34.0f}, "SAVE"))
    {
        showSavePopup = true;
        boardNameEditMode = true;
        errorMessage.clear();
    }
}

void BoardEditor::drawBoardGrid(Rectangle bounds)
{
    DrawRectangleRec(bounds, Fade(Theme::Background, 0.7f));
    DrawRectangleLinesEx(bounds, 2.0f, Theme::Border);

    const float cellW = bounds.width / static_cast<float>(cols);
    const float cellH = bounds.height / static_cast<float>(rows);

    renderer.drawEditorBoard(rows, cols, tiles, bounds, true);

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            Rectangle cell{bounds.x + col * cellW, bounds.y + row * cellH, cellW, cellH};
            const char tile = tileAt(row, col);
            DrawRectangleLinesEx(cell, 1.0f, Fade(Theme::Text, 0.35f));

            char label[2] = {tile, '\0'};
            const int fontSize = static_cast<int>(std::clamp(cellH * 0.45f, 12.0f, 24.0f));
            Color textColor = tile == '*' ? Fade(Theme::Text, 0.35f) : Theme::Text;
            if (tile == 'X') textColor = Fade(Theme::Background, 0.75f);
            drawCenteredText(label, cell, fontSize, textColor);
        }
    }

    Rectangle selected{
        bounds.x + selectedCol * cellW,
        bounds.y + selectedRow * cellH,
        cellW,
        cellH
    };
    DrawRectangleLinesEx(selected, 3.0f, Theme::AccentDark);

    if (!showSavePopup)
    {
        int row = 0;
        int col = 0;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && cellFromMouse(bounds, row, col))
        {
            costValueEditMode = false;
            costInputFresh = false;
            applyBrush(row, col);
        }
        else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && cellFromMouse(bounds, row, col))
        {
            costValueEditMode = false;
            costInputFresh = false;
            selectCell(row, col);
        }
    }
}

void BoardEditor::drawCostGrid(Rectangle bounds)
{
    DrawRectangleRec(bounds, Fade(Theme::Background, 0.7f));
    DrawRectangleLinesEx(bounds, 2.0f, Theme::Border);

    const float cellW = bounds.width / static_cast<float>(cols);
    const float cellH = bounds.height / static_cast<float>(rows);

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            Rectangle cell{bounds.x + col * cellW, bounds.y + row * cellH, cellW, cellH};
            const bool fixedObstacle = tileAt(row, col) == 'X';
            const bool selected = row == selectedRow && col == selectedCol;
            DrawRectangleRec(cell, fixedObstacle ? Fade(Theme::Border, 0.88f) : Theme::Background);
            if (selected && costValueEditMode && !fixedObstacle)
            {
                DrawRectangleRec(cell, Fade(Theme::Accent, 0.25f));
            }
            DrawRectangleLinesEx(cell, 1.0f, Fade(Theme::Text, 0.35f));

            const int fontSize = static_cast<int>(std::clamp(cellH * 0.28f, 8.0f, 15.0f));
            const char* label = TextFormat("%d", costAt(row, col));
            Color textColor = fixedObstacle ? Theme::Background : Theme::Text;
            drawCenteredText(label, cell, fontSize, textColor);
        }
    }

    Rectangle selected{
        bounds.x + selectedCol * cellW,
        bounds.y + selectedRow * cellH,
        cellW,
        cellH
    };
    DrawRectangleLinesEx(selected, 3.0f, Theme::AccentDark);

    if (!showSavePopup)
    {
        int row = 0;
        int col = 0;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && cellFromMouse(bounds, row, col))
        {
            selectCell(row, col);
            costValueEditMode = tileAt(row, col) != 'X';
            costInputFresh = costValueEditMode;
        }
    }
}

void BoardEditor::drawSavePopup()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(Theme::Text, 0.28f));

    Rectangle popup{
        GetScreenWidth() / 2.0f - 230.0f,
        GetScreenHeight() / 2.0f - 145.0f,
        460.0f,
        290.0f
    };

    DrawRectangleRec(popup, Theme::Background);
    DrawRectangleLinesEx(popup, 2.0f, Theme::Border);
    DrawText("SAVE BOARD", static_cast<int>(popup.x + 154), static_cast<int>(popup.y + 28), 24, Theme::Text);
    DrawText("Input board name", static_cast<int>(popup.x + 55), static_cast<int>(popup.y + 76), 18, Theme::Text);

    if (GuiTextBox(Rectangle{popup.x + 55.0f, popup.y + 105.0f, 350.0f, 42.0f}, boardName, sizeof(boardName), boardNameEditMode))
    {
        boardNameEditMode = !boardNameEditMode;
    }

    if (!errorMessage.empty())
    {
        DrawText(errorMessage.c_str(), static_cast<int>(popup.x + 55), static_cast<int>(popup.y + 160), 16, Theme::Error);
    }

    if (GuiButton(Rectangle{popup.x + 55.0f, popup.y + 215.0f, 140.0f, 45.0f}, "CANCEL"))
    {
        showSavePopup = false;
        boardNameEditMode = false;
        errorMessage.clear();
    }

    if (GuiButton(Rectangle{popup.x + 265.0f, popup.y + 215.0f, 140.0f, 45.0f}, "OK"))
    {
        std::string message;
        if (saveCurrentBoard(message))
        {
            statusMessage = message;
            showSavePopup = false;
            requestedScene = SceneType::SelectBoard;
        }
        else
        {
            errorMessage = message;
            boardNameEditMode = true;
        }
    }
}

bool BoardEditor::validateBoard(std::string& message) const
{
    int startCount = 0;
    int goalCount = 0;
    std::array<int, 10> digitCounts{};
    digitCounts.fill(0);

    for (char tile : tiles)
    {
        if (!isValidEditorTile(tile))
        {
            message = "Invalid tile exists on board.";
            return false;
        }
        if (tile == 'Z') ++startCount;
        else if (tile == 'O') ++goalCount;
        else if (isCheckpoint(tile)) ++digitCounts[tile - '0'];
    }

    if (startCount != 1)
    {
        message = "Board must contain exactly one Z start.";
        return false;
    }

    if (goalCount != 1)
    {
        message = "Board must contain exactly one O goal.";
        return false;
    }

    int highestDigit = -1;
    for (int i = 9; i >= 0; --i)
    {
        if (digitCounts[i] > 0)
        {
            highestDigit = i;
            break;
        }
    }

    for (int i = 0; i <= highestDigit; ++i)
    {
        if (digitCounts[i] != 1)
        {
            message = "Checkpoint digits must start from 0 and be contiguous.";
            return false;
        }
    }

    for (int i = 0; i < static_cast<int>(costs.size()); ++i)
    {
        if (tiles[i] == 'X') continue;
        if (costs[i] < 0)
        {
            message = "Tile costs must be non-negative.";
            return false;
        }
    }

    return true;
}

std::string BoardEditor::sanitizedBoardName() const
{
    std::string raw(boardName);
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

    output = trimExtension(output);
    return output;
}

bool BoardEditor::saveCurrentBoard(std::string& message)
{
    normalizeCosts();

    if (!validateBoard(message))
    {
        return false;
    }

    std::string name = sanitizedBoardName();
    if (name.empty())
    {
        message = "Board name cannot be empty.";
        return false;
    }

    try
    {
        std::filesystem::create_directories("./test");
        const std::string path = "./test/" + name + ".txt";
        std::ofstream output(path);
        if (!output.is_open())
        {
            message = "Failed to create " + path;
            return false;
        }

        output << rows << ' ' << cols << '\n';
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                output << tileAt(row, col);
            }
            output << '\n';
        }

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                if (col > 0) output << ' ';
                output << costAt(row, col);
            }
            output << '\n';
        }

        output.close();

        Board savedBoard = loadBoard(path);
        gameState.setBoard(savedBoard);
        gameState.setStringPath(path);
        gameState.setBoardSelected(true);
        gameState.setGameOver(false);
        gameState.setWinning(false);
        gameState.clearResult();

        message = "Saved " + path;
        return true;
    }
    catch (const std::exception& e)
    {
        message = e.what();
        return false;
    }
}

void BoardEditor::update()
{
    renderer.update(GetFrameTime(), true);

    if (showSavePopup)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            showSavePopup = false;
            boardNameEditMode = false;
            errorMessage.clear();
        }
        return;
    }

    if (costValueEditMode)
    {
        bool handledCostInput = false;

        if (tileAt(selectedRow, selectedCol) == 'X')
        {
            costValueEditMode = false;
            costInputFresh = false;
        }
        else
        {
            int& selectedCost = costs[indexOf(selectedRow, selectedCol)];
            for (int key = KEY_ZERO; key <= KEY_NINE; ++key)
            {
                if (IsKeyPressed(key))
                {
                    const int digit = key - KEY_ZERO;
                    selectedCost = costInputFresh ? digit : std::min(999999, selectedCost * 10 + digit);
                    costInputFresh = false;
                    handledCostInput = true;
                }
            }
            for (int key = KEY_KP_0; key <= KEY_KP_9; ++key)
            {
                if (IsKeyPressed(key))
                {
                    const int digit = key - KEY_KP_0;
                    selectedCost = costInputFresh ? digit : std::min(999999, selectedCost * 10 + digit);
                    costInputFresh = false;
                    handledCostInput = true;
                }
            }
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                selectedCost /= 10;
                costInputFresh = false;
                handledCostInput = true;
            }
            if (IsKeyPressed(KEY_DELETE))
            {
                selectedCost = 0;
                costInputFresh = false;
                handledCostInput = true;
            }
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
            {
                costValueEditMode = false;
                costInputFresh = false;
                handledCostInput = true;
            }
        }

        if (handledCostInput)
        {
            return;
        }
    }

    if (IsKeyPressed(KEY_X)) selectedBrush = 'X';
    if (IsKeyPressed(KEY_L)) selectedBrush = 'L';
    if (IsKeyPressed(KEY_Z)) selectedBrush = 'Z';
    if (IsKeyPressed(KEY_O)) selectedBrush = 'O';
    if (IsKeyPressed(KEY_SPACE)) selectedBrush = '*';
    if (!costValueEditMode)
    {
        for (int i = 0; i <= 9; ++i)
        {
            if (IsKeyPressed(KEY_ZERO + i)) selectedBrush = static_cast<char>('0' + i);
        }
    }

    if (IsKeyPressed(KEY_UP)) { selectCell(selectedRow - 1, selectedCol); costValueEditMode = false; costInputFresh = false; }
    if (IsKeyPressed(KEY_DOWN)) { selectCell(selectedRow + 1, selectedCol); costValueEditMode = false; costInputFresh = false; }
    if (IsKeyPressed(KEY_LEFT)) { selectCell(selectedRow, selectedCol - 1); costValueEditMode = false; costInputFresh = false; }
    if (IsKeyPressed(KEY_RIGHT)) { selectCell(selectedRow, selectedCol + 1); costValueEditMode = false; costInputFresh = false; }
    if (!costValueEditMode && IsKeyPressed(KEY_ENTER)) applyBrush(selectedRow, selectedCol);
}

void BoardEditor::draw()
{
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    if (showSavePopup) GuiLock();

    if (GuiButton(Rectangle{20.0f, 20.0f, 150.0f, 45.0f}, "Main Menu"))
    {
        gameState.reset();
        requestedScene = SceneType::MainMenu;
    }

    const char* title = "BOARD EDITOR";
    DrawText(
        title,
        screenW / 2 - MeasureText(title, 38) / 2,
        28,
        38,
        Theme::Text
    );

    const float scrollTop = 82.0f;
    Rectangle scrollBounds{
        0.0f,
        scrollTop,
        static_cast<float>(screenW),
        static_cast<float>(screenH) - scrollTop
    };

    const float minContentW = 1120.0f;
    const float contentW = std::max(minContentW, static_cast<float>(screenW));

    const float leftPanelW = 230.0f;
    const float outerPad = 20.0f;
    const float gap = 30.0f;

    const float localWorkX = outerPad + leftPanelW + gap;
    const float workW = contentW - localWorkX - outerPad;

    const float gridSizeByWidth = (workW - gap) / 2.0f;
    const float gridSizeByHeight = std::max(260.0f, static_cast<float>(screenH) - 230.0f);

    float gridSize = std::min(gridSizeByWidth, gridSizeByHeight);
    gridSize = std::clamp(gridSize, 260.0f, 760.0f);

    const float localGridY = 70.0f;

    const float contentH = std::max(
        static_cast<float>(screenH) - scrollTop + 20.0f,
        localGridY + gridSize + 170.0f
    );

    Rectangle content{
        0.0f,
        0.0f,
        contentW,
        contentH
    };

    Rectangle view{0.0f, 0.0f, 0.0f, 0.0f};
    GuiScrollPanel(scrollBounds, "", content, &editorScroll, &view);

    BeginScissorMode(
        static_cast<int>(view.x),
        static_cast<int>(view.y),
        static_cast<int>(view.width),
        static_cast<int>(view.height)
    );

    const float originX = scrollBounds.x + editorScroll.x;
    const float originY = scrollBounds.y + editorScroll.y;

    Rectangle leftPanel{
        originX + outerPad,
        originY + 20.0f,
        leftPanelW,
        contentH - 40.0f
    };

    drawLeftPanel(leftPanel);

    const float workX = originX + localWorkX;
    const float gridY = originY + localGridY;

    Rectangle boardArea{
        workX,
        gridY,
        gridSize,
        gridSize
    };

    Rectangle costArea{
        workX + gridSize + gap,
        gridY,
        gridSize,
        gridSize
    };

    DrawText("BOARD", static_cast<int>(boardArea.x), static_cast<int>(boardArea.y - 30.0f), 22, Theme::Text);
    DrawText("COST", static_cast<int>(costArea.x), static_cast<int>(costArea.y - 30.0f), 22, Theme::Text);

    Rectangle boardBounds = getBoardBounds(boardArea);
    Rectangle costBounds = getCostBounds(costArea);

    drawBoardGrid(boardBounds);
    drawCostGrid(costBounds);

    DrawText(
        "X cost is fixed to 999; other costs are editable.",
        static_cast<int>(costArea.x),
        static_cast<int>(costArea.y + costArea.height + 14.0f),
        16,
        Theme::Text
    );

    DrawText(
        TextFormat("Selected: row %d, col %d | Tile: %c | Cost: %d%s",
                   selectedRow + 1,
                   selectedCol + 1,
                   tileAt(selectedRow, selectedCol),
                   costAt(selectedRow, selectedCol),
                   costValueEditMode ? " | typing cost" : ""),
        static_cast<int>(workX),
        static_cast<int>(gridY + gridSize + 42.0f),
        16,
        Theme::Text
    );

    if (tileAt(selectedRow, selectedCol) == 'X')
    {
        costs[indexOf(selectedRow, selectedCol)] = 999;
    }

    DrawText(
        statusMessage.c_str(),
        static_cast<int>(workX),
        static_cast<int>(gridY + gridSize + 68.0f),
        17,
        Theme::Text
    );

    if (!errorMessage.empty() && !showSavePopup)
    {
        DrawText(
            errorMessage.c_str(),
            static_cast<int>(workX),
            static_cast<int>(gridY + gridSize + 92.0f),
            16,
            Theme::Error
        );
    }

    if (GuiButton(
        Rectangle{
            workX + (gridSize * 2.0f + gap) / 2.0f - 105.0f,
            gridY + gridSize + 112.0f,
            210.0f,
            42.0f
        },
        "SAVE BOARD"
    ))
    {
        showSavePopup = true;
        boardNameEditMode = true;
        errorMessage.clear();
    }

    EndScissorMode();

    if (showSavePopup)
    {
        GuiUnlock();
        drawSavePopup();
    }
}

SceneType BoardEditor::nextScene()
{
    return requestedScene;
}
