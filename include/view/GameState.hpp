#pragma once
#include <string>
#include "utils/Bridge.hpp"

enum class PlayMode
{
    MANUAL,
    ALGORITHM,
    CREATE_MAP,
    SENTINEL
};
class GameState
{
public:
    GameState();
    GameState(GameState& other) = delete;
    GameState& operator=(const GameState& other) = delete;

    bool isPlayerSelected() const;
    PlayMode getPlayMode() const;
    Board getBoard() const;
    const Board& getBoardRef() const;
    bool isBoardSelected() const;
    const char* getStringPath() const;
    const std::string& getPlayerType() const;
    bool isGameOver() const;
    bool isWinning() const;
    bool isResultGenerated() const;
    bool isHasResult() const;
    SolveResult getResult() const;
    const char* getErrorMessage() const;

    void setPlayMode(PlayMode p);
    void setBoard(const Board& b);
    void setBoardSelected(bool nVal);
    void setStringPath(std::string newPath);
    void setPlayerSelected(bool nVal);
    void setPlayerType(std::string newType);
    void setGameOver(bool nVal);
    void setWinning(bool nVal);
    void setIsResultGenerated(bool nVal);
    void setIsHasResult(bool nVal);
    void setResult(SolveResult newRes);
    void setErrorMessage(std::string errorMsg);

    void clearErrMsg();
    void clearBoard();
    void clearResult();
    void reset();
private:
    PlayMode playMode;

    bool boardSelected;
    std::string selectedBoardPath;
    Board board;

    bool playerSelected;
    std::string playerType;
    bool gameOver;
    bool winning;
    bool resultGenerated;
    SolveResult result;
    bool hasResult;

    std::string errorMessage;
};
