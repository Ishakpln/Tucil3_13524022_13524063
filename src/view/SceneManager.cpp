#include "view/SceneManager.hpp"

SceneManager::SceneManager(GameState& gs, GameController& gc):
    gameState(gs),
    gameController(gc),
    currentSceneType(SceneType::MainMenu),
    currentScene(createScene(SceneType::MainMenu)){}

std::unique_ptr<Scene> SceneManager::createScene(SceneType sType)
{
    switch (sType)
    {
        case SceneType::MainMenu:
            return std::make_unique<MainMenu>(this->gameState);
        case SceneType::ChoosePlayer:
            return std::make_unique<ChoosePlayer>(this->gameState);
        case SceneType::SelectBoard:
            return std::make_unique<SelectBoard>(this->gameState);
        case SceneType::AlgoPlay:
            return std::make_unique<AlgoPlay>(this->gameState);
        case SceneType::ManualPlay:
            return std::make_unique<ManualPlay>(this->gameState);
        case SceneType::BoardEditor:
            return std::make_unique<BoardEditor>(this->gameState);
    }

    return std::make_unique<MainMenu>(this->gameState);
}

void SceneManager::update()
{
    this->currentScene->update();

    if (this->currentSceneType != this->currentScene->nextScene())
    {
        this->changeScene(this->currentScene->nextScene());
    }
}
void SceneManager::changeScene(SceneType ns)
{
    this->currentSceneType = ns;
    this->currentScene = createScene(this->currentSceneType);
}
void SceneManager::draw()
{
    this->currentScene->draw();
}