#include "view/scenes/ChoosePlayer.hpp"
#include <iostream>
#define IMAGE_SCALE 1.5
#define ARROW_GAP 300
#define ARROW_HEIGHT 200
#define ARROW_WIDTH 200
#define TITLE_SIZE 40
#define PLAYER_SIZE 200

ChoosePlayer::ChoosePlayer(GameState& gs):
    gameState(gs),
    lBtn((Rectangle){0, 1920/3.0f, 1920/2, 1920/3}, (Rectangle){(GetScreenWidth() - ARROW_GAP)/2.0F - ARROW_WIDTH, (GetScreenHeight() - ARROW_HEIGHT)/2.0f, ARROW_WIDTH, ARROW_HEIGHT}, "./assets/components/arrow.png", (Vector2){0,0}),
    rBtn((Rectangle){1920/2, 1920/3.0f, 1920/2, 1920/3}, (Rectangle){(GetScreenWidth() + ARROW_GAP)/2.0F, (GetScreenHeight() - ARROW_HEIGHT)/2.0f, ARROW_WIDTH, ARROW_HEIGHT}, "./assets/components/arrow.png", (Vector2){0,0}),
    pImage((Rectangle){0,0, 600, 600}, (Rectangle){(GetScreenWidth() - PLAYER_SIZE)/2.0F, (GetScreenHeight() - PLAYER_SIZE)/2.0f, PLAYER_SIZE, PLAYER_SIZE}, "./assets/players/Baby/1.png", (Vector2){0,0}),
    title((GetScreenWidth() - MeasureText("Choose Your Player", TITLE_SIZE))/2.0f, GetScreenHeight()/6.0f, "Choose Your Player", TITLE_SIZE),
    rlmove(false)
    {}

void ChoosePlayer::update()
{
    this->lBtn.setDestBounds((Rectangle){(GetScreenWidth() - ARROW_GAP)/2.0F - ARROW_WIDTH, (GetScreenHeight() - ARROW_HEIGHT)/2.0f, ARROW_WIDTH, ARROW_HEIGHT});
    this->rBtn.setDestBounds((Rectangle){(GetScreenWidth() + ARROW_GAP)/2.0F, (GetScreenHeight() - ARROW_HEIGHT)/2.0f, ARROW_WIDTH, ARROW_HEIGHT});
    this->title.setPosition((GetScreenWidth() - MeasureText("Choose Your Player", TITLE_SIZE))/2.0f, GetScreenHeight()/6.0f);
    this->pImage.setDestBounds((Rectangle){(GetScreenWidth() - PLAYER_SIZE)/2.0F, (GetScreenHeight() - PLAYER_SIZE)/2.0f, PLAYER_SIZE, PLAYER_SIZE});

    std::string path1 = "./assets/players/Ball/Ball1.png";
    std::string path2 = "./assets/players/Baby/1.png";
    if (this->lBtn.isClicked()) { this->rlmove = !this->rlmove; }
    if (this->rBtn.isClicked()) { this->rlmove = !this->rlmove; }

    if (this->rlmove) this->pImage.setImagePath(path1.c_str());
    else this->pImage.setImagePath(path2.c_str());
}
void ChoosePlayer::draw()
{
    this->lBtn.draw();
    this->rBtn.draw();
    this->title.draw();
    this->pImage.draw();
}
SceneType ChoosePlayer::nextScene()
{
    if (this->gameState.isPlayerSelected())
    {
        if (this->gameState.getPlayMode() == PlayMode::ALGORITHM) return SceneType::AlgoPlay;
        else if (this->gameState.getPlayMode() == PlayMode::MANUAL) return SceneType::ManualPlay;
        else return SceneType::MainMenu;
    }
    else
    {
        return SceneType::ChoosePlayer;
    }
}