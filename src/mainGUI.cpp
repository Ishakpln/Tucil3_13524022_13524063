#include "raylib.h"
// #include "view/AppState.hpp"

int main()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TEST_WINDOW");

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello World", 400, 300, 50, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}