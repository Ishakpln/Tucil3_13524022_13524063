#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/components/MenuButton.hpp"
#include "view/components/Title.hpp"

#define BUTTON_GAP 10
#define BUTTON_HEIGHT 100
#define BUTTON_WIDTH 400
#define TITLE_SIZE 40
#define TEXT_BTN_SIZE 28
class MainMenu: public Scene
{
private:
    GameState& gameState;
    MenuButton manualBtn;
    MenuButton algoBtn;
    MenuButton cmapBtn;
    Title title;

public:
    MainMenu(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};
