#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"

class SelectBoard: public Scene
{
private:
    GameState& gameState;

public:
    SelectBoard(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};