#include "view/GameState.hpp"

GameState::GameState(): 
    playMode(PlayMode::SENTINEL),
    boardSelected(false),
    selectedBoardPath("null"),
    playerSelected(false),
    playerImagePath("null"),
    board(),
    gameOver(false),
    winning(false),
    resultGenerated(false),
    result(),
    hasResult(false),
    errorMessage("null"){}

PlayMode GameState::getPlayMode() const { return this->playMode; }
Board GameState::getBoard() const {return this->board;}
bool GameState::isBoardSelected() const {return this->boardSelected;}
const char* GameState::getStringPath() const {return this->selectedBoardPath.c_str();}
bool GameState::isPlayerSelected() const {return this->playerSelected;}
bool GameState::isGameOver() const {return this->gameOver;}
bool GameState::isWinning() const {return this->winning;}
bool GameState::isResultGenerated() const{return this->resultGenerated;}
bool GameState::isHasResult() const {return this->hasResult;}
SolveResult GameState::getResult() const {return this->result;}
const char* GameState::getErrorMessage() const {return this->errorMessage.c_str();}
void GameState::setPlayMode(PlayMode p) { this->playMode = p; }
void GameState::setBoard(Board& b) { this->board = b; }
bool GameState::setBoardSelected(bool nVal) { this->boardSelected = nVal; }
void GameState::setStringPath(std::string newPath) { this->selectedBoardPath = newPath; }
void GameState::setGameOver(bool nVal) { this->gameOver = nVal; }
void GameState::setWinning(bool nVal) { this->winning = nVal; }
void GameState::setIsResultGenerated(bool nVal) { this->resultGenerated = nVal; }
void GameState::setIsHasResult(bool nVal) { this->hasResult = nVal; }
void GameState::setResult(SolveResult newRes) { this->result = newRes; }
void GameState::setErrorMessage(std::string errorMsg) { this->errorMessage = errorMsg; }
void GameState::clearErrMsg() { this->errorMessage = "null"; }
void GameState::clearBoard() { this->board = Board(); }
void GameState::clearResult() { this->result = SolveResult(); }
void GameState::reset()
{
    this->playMode = PlayMode::SENTINEL;
    this->boardSelected = false;
    this->selectedBoardPath = "null";
    this->board = Board();
    this->gameOver = false;
    this->winning = false;
    this->resultGenerated = false;
    this->result = SolveResult();
    this->hasResult = false;
    this->errorMessage = "null";
}
