#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/TileMotion.hpp"
#include "view/assets/BoardRenderer.hpp"

class ManualPlay : public Scene
{
private:
    GameState& gameState;
    BoardRenderer renderer;
    TileMotion playerMotion;
    Point playerPosition;
    int targetIndex;
    SceneType requestedScene;
    std::string statusMessage;

    bool tryMove(Direction direction);

public:
    ManualPlay(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};
