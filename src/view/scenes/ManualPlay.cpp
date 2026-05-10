#include "view/scenes/ManualPlay.hpp"
#include "library/raygui.h"
#include "utils/GuiHelper.hpp"

ManualPlay::ManualPlay(GameState &gs): 
    gameState(gs),
    renderer(gs.getPlayerType()),
    playerMotion(3.0f),
    playerPosition(gs.isBoardSelected() ? gs.getBoardRef().getStartPosition() : Point{-1, -1}),
    targetIndex(0),
    requestedScene(SceneType::ManualPlay),
    statusMessage("Use W, A, S, D to move") {
    playerMotion.reset(playerPosition);
}

bool ManualPlay::tryMove(Direction direction)
{
    if (!gameState.isBoardSelected() || gameState.isGameOver() || gameState.isWinning() || playerMotion.isActive()) {
        return false;
    }

    Point oldPosition = playerPosition;

    Node node{playerPosition, targetIndex, 0, 0.0f, 0.0f, -1, direction};
    SlideResult slide = gameState.getBoardRef().slideTo(node, direction);

    playerPosition = slide.position;
    targetIndex = slide.targetIndex;

    bool moved = oldPosition != playerPosition;
    if (moved) {
        playerMotion.start(oldPosition, playerPosition);
    }
    else {
        playerMotion.reset(playerPosition);
    }

    if (slide.isFinished)
    {
        gameState.setWinning(true);
        statusMessage = "You win!";
    }
    else if (slide.isGameOver)
    {
        gameState.setGameOver(true);
        statusMessage = "Game over";
    }
    else
    {
        statusMessage = TextFormat("Cost: %d | Target index: %d", slide.cost, targetIndex);
    }

    return moved;
}

void ManualPlay::update() {
    float dt = GetFrameTime();
    playerMotion.update(dt);
    renderer.update(dt, playerMotion.isActive());

    // While the player is sliding, ignore new movement input so visual and logical state stay in sync.
    if (playerMotion.isActive()) {
        return;
    }

    if (IsKeyPressed(KEY_W)) tryMove(Direction::U);
    if (IsKeyPressed(KEY_A)) tryMove(Direction::L);
    if (IsKeyPressed(KEY_S)) tryMove(Direction::D);
    if (IsKeyPressed(KEY_D)) tryMove(Direction::R);
}

void ManualPlay::draw()
{
    DrawText("PLAY MANUAL", 210, 25, 32, Theme::Text);

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
    if (GuiButton(Rectangle{20, 185, 150, 42}, "Retry"))
    {
        playerPosition = gameState.getBoardRef().getStartPosition();
        playerMotion.reset(playerPosition);
        targetIndex = 0;
        gameState.setGameOver(false);
        gameState.setWinning(false);
        statusMessage = "Use W, A, S, D to move";
    }

    Rectangle boardBounds{200.0f, 80.0f, GetScreenWidth() - 230.0f, GetScreenHeight() - 140.0f};
    if (gameState.isBoardSelected())
    {
        Vector2 drawPosition = playerMotion.getCurrentTilePosition();
        renderer.drawBoardAt(gameState.getBoardRef(), boardBounds, drawPosition.x, drawPosition.y, true, playerMotion.getRotationFromUpDegrees());
    }
    else
    {
        DrawText("No board selected", static_cast<int>(boardBounds.x), static_cast<int>(boardBounds.y), 24, Theme::Error);
    }

    DrawText(statusMessage.c_str(), 200, GetScreenHeight() - 45, 20, Theme::Text);
}

SceneType ManualPlay::nextScene()
{
    return requestedScene;
}
