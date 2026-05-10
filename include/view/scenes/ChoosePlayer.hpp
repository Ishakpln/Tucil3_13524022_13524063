#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/components/Title.hpp"
#include "view/components/ImageTexture.hpp"
class ChoosePlayer : public Scene
{
private:
    GameState &gameState;
    ImageTexture lBtn;
    ImageTexture rBtn;
    ImageTexture pImage;
    Title title;
    bool rlmove;

public:
    ChoosePlayer(GameState &gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};