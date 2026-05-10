#pragma once
#include "view/Scene.hpp"
#include "view/GameState.hpp"
#include "view/assets/BoardRenderer.hpp"

class AlgoPlay : public Scene
{
private:
    GameState& gameState;
    BoardRenderer renderer;
    SceneType requestedScene;
    AlgorithmType selectedAlgorithm;
    HeuristicType selectedHeuristic;
    int currentStep;
    float playbackTimer;
    bool playing;
    std::string statusMessage;

    Point currentPlayerPosition() const;
    void runSolver();

public:
    AlgoPlay(GameState& gs);
    void update() override;
    void draw() override;
    SceneType nextScene() override;
};
