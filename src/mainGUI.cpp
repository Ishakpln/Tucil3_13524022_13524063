#include "raylib.h"
#include "library/raygui.h"
#include "view/GameController.hpp"
#include "view/SceneManager.hpp"
#include "utils/GuiHelper.hpp"

int main()
{
    const int SCREEN_WIDTH = 1024;
    const int SCREEN_HEIGHT = 768;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ice Sliding Puzzle Solver");
    ApplyGuiTheme();
    SetTargetFPS(60);

    GameState gs;
    GameController gc;
    SceneManager sceneManager(gs, gc);

    while (!WindowShouldClose())
    {
        sceneManager.update();
        BeginDrawing();
        ClearBackground(Theme::Background);
        sceneManager.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
