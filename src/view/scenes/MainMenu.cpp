#include "view/scenes/MainMenu.hpp"

MainMenu::MainMenu(GameState& gs):
    gameState(gs),
    manualBtn({500, 280, 200, 50}, 0, "Play Manual", BLACK, BLACK),
    algoBtn({500, 420, 200, 50}, 159, "Play Algorithm", BLACK, BLACK),
    cmapBtn({500, 350, 200, 50}, 96, "Map Editor", BLACK, BLACK),
    title(300, 10, "Ice Sliding"){}
void MainMenu::update(){}
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
