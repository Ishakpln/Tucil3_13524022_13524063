#pragma once

enum class SceneType
{
    MainMenu,
    ChoosePlayer,
    SelectMap,
    ManualPlay,
    AlgoPlay,
    MapEditor
};

class Scene
{
    public:
        ~Scene() = default;
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual SceneType nextScene() = 0;
};