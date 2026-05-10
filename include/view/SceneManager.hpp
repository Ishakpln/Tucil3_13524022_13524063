#pragma once
#include <memory>
#include "view/scenes/MainMenu.hpp"
#include "view/scenes/AlgoPlay.hpp"
#include "view/scenes/ManualPlay.hpp"
#include "view/scenes/BoardEditor.hpp"
#include "view/scenes/ChoosePlayer.hpp"
#include "view/scenes/SelectBoard.hpp"
#include "view/GameController.hpp"

class SceneManager
{
private:
    GameState& gameState;
    GameController& gameController;

    SceneType currentSceneType;
    std::unique_ptr<Scene> currentScene;

    std::unique_ptr<Scene> createScene(SceneType s);
    void changeScene(SceneType ns);

public:
    SceneManager(GameState& gs, GameController& gc);
    void update();
    void draw();
};