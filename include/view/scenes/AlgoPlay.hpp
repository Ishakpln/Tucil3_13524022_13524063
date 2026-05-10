#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/TileMotion.hpp"
#include "view/assets/BoardRenderer.hpp"

class AlgoPlay : public Scene
{
private:
    GameState& gameState;
    BoardRenderer renderer;
    TileMotion playerMotion;
    SceneType requestedScene;
    AlgorithmType selectedAlgorithm;
    HeuristicType selectedHeuristic;
    bool algorithmDropdownEditMode;
    bool heuristicDropdownEditMode;
    int currentStep;
    float playbackTimer;
    bool playing;
    std::string statusMessage;

    Point currentPlayerPosition() const;
    int currentTargetIndex() const;
    void runSolver();
    void moveToStep(int nextStep);

public:
    AlgoPlay(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};
