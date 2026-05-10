#include "view/scenes/BoardEditor.hpp"
#include "library/raygui.h"
#include "utils/GuiHelper.hpp"

BoardEditor::BoardEditor(GameState &gs) : gameState(gs), requestedScene(SceneType::BoardEditor) {}

void BoardEditor::update() {}
void BoardEditor::draw()
{
    DrawText("BOARD EDITOR", 240, 30, 36, Theme::Text);
    DrawText("Editor placeholder: use existing text files in ./test for now.", 120, 150, 22, Theme::Border);
    DrawText("Logical chars: X obstacle, * path, L lava, Z start, O goal, 0-9 checkpoints", 80, 190, 20, Theme::Border);

    if (GuiButton(Rectangle{20, 20, 150, 45}, "Main Menu"))
    {
        gameState.reset();
        requestedScene = SceneType::MainMenu;
    }
    if (GuiButton(Rectangle{GetScreenWidth() / 2.0f - 100.0f, GetScreenHeight() - 90.0f, 200.0f, 50.0f}, "Back to Boards"))
    {
        requestedScene = SceneType::SelectBoard;
    }
}
SceneType BoardEditor::nextScene() { return requestedScene; }
