#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/components/MenuButton.hpp"
#include "view/components/Title.hpp"

class MainMenu: public Scene
{
private:
    GameState& gameState;
    MenuButton manualBtn;
    MenuButton algoBtn;
    MenuButton cmapBtn;
    Title title;
    SceneType requestedScene;

public:
    MainMenu(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};
