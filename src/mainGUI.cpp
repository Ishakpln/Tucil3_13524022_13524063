#include "raylib.h"
#include "library/raygui.h"
#include "view/GameController.hpp"
#include "view/SceneManager.hpp"
#include "view/components/ImageTexture.hpp"
#include "view/components/DescriptionPanel.hpp"
#include "view/components/MenuButton.hpp"
#include "view/components/Popup.hpp"
#include "view/components/Title.hpp"
#include "view/components/TransitionEffect.hpp"

int main()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TEST_WINDOW");

    GameState gs;
    GameController gc;
    SceneManager sceneManager(gs, gc);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        sceneManager.update();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        sceneManager.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}