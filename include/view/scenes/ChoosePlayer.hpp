#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/components/Title.hpp"
#include "view/assets/Player.hpp"
#include <memory>
#include <string>
#include <vector>

class ChoosePlayer : public Scene
{
private:
    GameState &gameState;
    Title title;
    std::vector<std::string> playerTypes;
    int selectedIndex;
    std::unique_ptr<Player> previewPlayer;
    SceneType requestedScene;

    void rebuildPreviewPlayer();
    SceneType playgroundScene() const;

public:
    ChoosePlayer(GameState &gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};
