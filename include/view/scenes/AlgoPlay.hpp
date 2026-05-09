#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"

class AlgoPlay : public Scene
{
private:
    GameState& gameState;

public:
    AlgoPlay(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};