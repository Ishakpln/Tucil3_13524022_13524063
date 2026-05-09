#pragma once

enum class SceneType
{
    MainMenu,
    ChoosePlayer,
    SelectBoard,
    ManualPlay,
    AlgoPlay,
    BoardEditor
};

class Scene
{
public:
    ~Scene() = default;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual SceneType nextScene() = 0;
};