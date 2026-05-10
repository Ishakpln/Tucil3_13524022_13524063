#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/assets/BoardRenderer.hpp"

class ManualPlay : public Scene
{
private:
    GameState& gameState;
    BoardRenderer renderer;
    Point playerPosition;
    int targetIndex;
    SceneType requestedScene;
    std::string statusMessage;
    float playerMoveAnimTimer;

    bool tryMove(Direction direction);

public:
    ManualPlay(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};
