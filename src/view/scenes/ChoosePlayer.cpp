#include "view/scenes/ChoosePlayer.hpp"
#include "library/raygui.h"
#define TITLE_SIZE 40
#define PLAYER_SIZE 200
#define BUTTON_W 150
#define BUTTON_H 50

ChoosePlayer::ChoosePlayer(GameState& gs):
    gameState(gs),
    title((GetScreenWidth() - MeasureText("Choose Player", TITLE_SIZE))/2.0f, GetScreenHeight()/6.0f, "Choose Player", TITLE_SIZE),
    playerTypes({"Baby", "Fire"}),
    selectedIndex(0),
    previewPlayer(nullptr),
    requestedScene(SceneType::ChoosePlayer)
{
    for (int i = 0; i < static_cast<int>(playerTypes.size()); ++i) {
        if (playerTypes[i] == gameState.getPlayerType()) {
            selectedIndex = i;
            break;
        }
    }
    rebuildPreviewPlayer();
}

void ChoosePlayer::rebuildPreviewPlayer() {
    previewPlayer = createPlayerByType(playerTypes[selectedIndex]);
}

SceneType ChoosePlayer::playgroundScene() const {
    return gameState.getPlayMode() == PlayMode::ALGORITHM ? SceneType::AlgoPlay : SceneType::ManualPlay;
}

void ChoosePlayer::update()
{
    this->title.setPosition((GetScreenWidth() - MeasureText("Choose Player", TITLE_SIZE))/2.0f, GetScreenHeight()/6.0f);
    if (previewPlayer) {
        previewPlayer->update(GetFrameTime(), true);
    }
}
void ChoosePlayer::draw()
{
    const float centerX = GetScreenWidth() / 2.0f;
    const float centerY = GetScreenHeight() / 2.0f;

    if (GuiButton(Rectangle{20, 20, 150, 45}, "Main Menu")) {
        gameState.reset();
        requestedScene = SceneType::MainMenu;
    }

    this->title.draw();

    if (GuiButton(Rectangle{centerX - 270.0f, centerY - 25.0f, 80.0f, 80.0f}, "<")) {
        selectedIndex = (selectedIndex - 1 + static_cast<int>(playerTypes.size())) % static_cast<int>(playerTypes.size());
        rebuildPreviewPlayer();
    }
    if (GuiButton(Rectangle{centerX + 190.0f, centerY - 25.0f, 80.0f, 80.0f}, ">")) {
        selectedIndex = (selectedIndex + 1) % static_cast<int>(playerTypes.size());
        rebuildPreviewPlayer();
    }

    DrawText(playerTypes[selectedIndex].c_str(), static_cast<int>(centerX - MeasureText(playerTypes[selectedIndex].c_str(), 26) / 2.0f), static_cast<int>(centerY - 170.0f), 26, BLACK);

    if (previewPlayer) {
        previewPlayer->drawAt(centerX - PLAYER_SIZE / 2.0f, centerY - PLAYER_SIZE / 2.0f, PLAYER_SIZE, 0, 0);
    }

    if (GuiButton(Rectangle{centerX - BUTTON_W / 2.0f, centerY + 180.0f, BUTTON_W, BUTTON_H}, "OK")) {
        gameState.setPlayerType(playerTypes[selectedIndex]);
        gameState.setPlayerSelected(true);
        requestedScene = gameState.isBoardSelected() ? playgroundScene() : SceneType::SelectBoard;
    }
}
SceneType ChoosePlayer::nextScene()
{
    return requestedScene;
}
