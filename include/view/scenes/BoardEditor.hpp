#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"

class BoardEditor : public Scene
{
private:
    GameState& gameState;

public:
    BoardEditor(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};