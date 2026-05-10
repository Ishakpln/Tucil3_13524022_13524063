#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/assets/BoardRenderer.hpp"
#include <string>
#include <vector>

class SelectBoard: public Scene
{
private:
    GameState& gameState;
    BoardRenderer renderer;
    std::vector<std::string> boardPaths;
    int selectedIndex;
    Board previewBoard;
    bool hasPreview;
    SceneType requestedScene;
    std::string errorMessage;

    void discoverBoards();
    void loadPreview();
    SceneType playgroundScene() const;

public:
    SelectBoard(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};
