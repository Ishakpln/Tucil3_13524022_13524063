#include "view/scenes/MainMenu.hpp"

MainMenu::MainMenu(GameState& gs):
    gameState(gs),
    manualBtn((Rectangle){(GetScreenWidth() - BUTTON_WIDTH)/2.0f, GetScreenHeight()/3.0f, BUTTON_WIDTH, BUTTON_HEIGHT}, 131, "Play Manual", BLACK, BLACK, TEXT_BTN_SIZE),
    algoBtn((Rectangle){(GetScreenWidth() - BUTTON_WIDTH)/2.0f, GetScreenHeight()/3.0f + BUTTON_HEIGHT + BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT}, 159, "Play Algorithm", BLACK, BLACK, TEXT_BTN_SIZE),
    cmapBtn((Rectangle){(GetScreenWidth() - BUTTON_WIDTH)/2.0f, GetScreenHeight()/3.0f + (BUTTON_HEIGHT + BUTTON_GAP)*2 , BUTTON_WIDTH, BUTTON_HEIGHT}, 96, "Map Editor", BLACK, BLACK, TEXT_BTN_SIZE),
    title((GetScreenWidth() + MeasureText("Ice Sliding", TITLE_SIZE))/2, GetScreenHeight()/6.0f, "Ice Sliding", TITLE_SIZE){}

void MainMenu::update()
{
    this->manualBtn.setBounds((Rectangle){(GetScreenWidth() - BUTTON_WIDTH)/2.0f, GetScreenHeight()/3.0f, BUTTON_WIDTH, BUTTON_HEIGHT});
    this->algoBtn.setBounds((Rectangle){(GetScreenWidth() - BUTTON_WIDTH)/2.0f, GetScreenHeight()/3.0f + BUTTON_HEIGHT + BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT});
    this->cmapBtn.setBounds((Rectangle){(GetScreenWidth() - BUTTON_WIDTH)/2.0f, GetScreenHeight()/3.0f + (BUTTON_HEIGHT + BUTTON_GAP)*2, BUTTON_WIDTH, BUTTON_HEIGHT});
    this->title.setPosition((GetScreenWidth() - MeasureText("Ice Sliding", TITLE_SIZE))/2, GetScreenHeight()/6.0f);
}
void MainMenu::draw()
{
    title.draw();
    if(manualBtn.draw()) std::cout << "Manual pressed\n";
    if(algoBtn.draw()) std::cout << "Algo pressed\n";
    if(cmapBtn.draw()) std::cout << "Create Map pressed\n";
}
SceneType MainMenu::nextScene()
{
    switch (this->gameState.getPlayMode())
    {
        case PlayMode::MANUAL:
            return SceneType::ChoosePlayer;
        case PlayMode::ALGORITHM:
            return SceneType::ChoosePlayer;
        case PlayMode::CREATE_MAP:
            return SceneType::BoardEditor;
        case PlayMode::SENTINEL:
            return SceneType::MainMenu;
    }

}
