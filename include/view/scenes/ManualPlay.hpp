#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"

class ManualPlay : public Scene
{
private:
    GameState& gameState;

public:
    ManualPlay(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};