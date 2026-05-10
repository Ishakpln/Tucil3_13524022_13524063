#include "view/scenes/MainMenu.hpp"
#include "utils/GuiHelper.hpp"
#define BUTTON_GAP 10
#define BUTTON_HEIGHT 100
#define BUTTON_WIDTH 400
#define TITLE_SIZE 40
#define TEXT_BTN_SIZE 28

MainMenu::MainMenu(GameState &gs): 
    gameState(gs),
    manualBtn((Rectangle){(GetScreenWidth() - BUTTON_WIDTH) / 2.0f, GetScreenHeight() / 3.0f, BUTTON_WIDTH, BUTTON_HEIGHT}, 131, "Play Manual", Theme::Text, Theme::Text, TEXT_BTN_SIZE),
    algoBtn((Rectangle){(GetScreenWidth() - BUTTON_WIDTH) / 2.0f, GetScreenHeight() / 3.0f + BUTTON_HEIGHT + BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT}, 159, "Play Algorithm", Theme::Text, Theme::Text, TEXT_BTN_SIZE),
    cmapBtn((Rectangle){(GetScreenWidth() - BUTTON_WIDTH) / 2.0f, GetScreenHeight() / 3.0f + (BUTTON_HEIGHT + BUTTON_GAP) * 2, BUTTON_WIDTH, BUTTON_HEIGHT}, 96, "Create Board", Theme::Text, Theme::Text, TEXT_BTN_SIZE),
    title((GetScreenWidth() - MeasureText("ICE SLIDING", TITLE_SIZE)) / 2, GetScreenHeight() / 6.0f, "ICE SLIDING", TITLE_SIZE),
    requestedScene(SceneType::MainMenu) {}

void MainMenu::update()
{
    this->manualBtn.setBounds((Rectangle){(GetScreenWidth() - BUTTON_WIDTH) / 2.0f, GetScreenHeight() / 3.0f, BUTTON_WIDTH, BUTTON_HEIGHT});
    this->algoBtn.setBounds((Rectangle){(GetScreenWidth() - BUTTON_WIDTH) / 2.0f, GetScreenHeight() / 3.0f + BUTTON_HEIGHT + BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT});
    this->cmapBtn.setBounds((Rectangle){(GetScreenWidth() - BUTTON_WIDTH) / 2.0f, GetScreenHeight() / 3.0f + (BUTTON_HEIGHT + BUTTON_GAP) * 2, BUTTON_WIDTH, BUTTON_HEIGHT});
    this->title.setPosition((GetScreenWidth() - MeasureText("ICE SLIDING", TITLE_SIZE)) / 2, GetScreenHeight() / 6.0f);
}
void MainMenu::draw()
{
    title.draw();
    if (manualBtn.draw())
    {
        this->gameState.reset();
        this->gameState.setPlayMode(PlayMode::MANUAL);
        this->requestedScene = SceneType::ChoosePlayer;
    }
    if (algoBtn.draw())
    {
        this->gameState.reset();
        this->gameState.setPlayMode(PlayMode::ALGORITHM);
        this->requestedScene = SceneType::ChoosePlayer;
    }
    if (cmapBtn.draw())
    {
        this->requestedScene = SceneType::BoardEditor;
    }
}
SceneType MainMenu::nextScene()
{
    return requestedScene;
}
