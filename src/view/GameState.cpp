#include "view/GameState.hpp"

GameState::GameState(): 
    playMode(PlayMode::SENTINEL),
    boardSelected(false),
    selectedBoardPath("null"),
    board(),
    playerSelected(false),
    playerType("Baby"),
    gameOver(false),
    winning(false),
    resultGenerated(false),
    result(),
    hasResult(false),
    errorMessage("null"){}

PlayMode GameState::getPlayMode() const { return this->playMode; }
Board GameState::getBoard() const {return this->board;}
const Board& GameState::getBoardRef() const {return this->board;}
bool GameState::isBoardSelected() const {return this->boardSelected;}
const char* GameState::getStringPath() const {return this->selectedBoardPath.c_str();}
bool GameState::isPlayerSelected() const {return this->playerSelected;}
const std::string& GameState::getPlayerType() const {return this->playerType;}
bool GameState::isGameOver() const {return this->gameOver;}
bool GameState::isWinning() const {return this->winning;}
bool GameState::isResultGenerated() const{return this->resultGenerated;}
bool GameState::isHasResult() const {return this->hasResult;}
SolveResult GameState::getResult() const {return this->result;}
const char* GameState::getErrorMessage() const {return this->errorMessage.c_str();}
void GameState::setPlayMode(PlayMode p) { this->playMode = p; }
void GameState::setBoard(const Board& b) { this->board = b; }
void GameState::setBoardSelected(bool nVal) { this->boardSelected = nVal; }
void GameState::setStringPath(std::string newPath) { this->selectedBoardPath = newPath; }
void GameState::setPlayerSelected(bool nVal) { this->playerSelected = nVal; }
void GameState::setPlayerType(std::string newType) { this->playerType = newType.empty() ? "Baby" : newType; }
void GameState::setGameOver(bool nVal) { this->gameOver = nVal; }
void GameState::setWinning(bool nVal) { this->winning = nVal; }
void GameState::setIsResultGenerated(bool nVal) { this->resultGenerated = nVal; }
void GameState::setIsHasResult(bool nVal) { this->hasResult = nVal; }
void GameState::setResult(SolveResult newRes) { this->result = newRes; }
void GameState::setErrorMessage(std::string errorMsg) { this->errorMessage = errorMsg; }
void GameState::clearErrMsg() { this->errorMessage = "null"; }
void GameState::clearBoard() { this->board = Board(); this->boardSelected = false; this->selectedBoardPath = "null"; }
void GameState::clearResult() { this->result = SolveResult(); this->hasResult = false; this->resultGenerated = false; }
void GameState::reset()
{
    this->playMode = PlayMode::SENTINEL;
    this->boardSelected = false;
    this->selectedBoardPath = "null";
    this->board = Board();
    this->playerSelected = false;
    this->playerType = "Baby";
    this->gameOver = false;
    this->winning = false;
    this->resultGenerated = false;
    this->result = SolveResult();
    this->hasResult = false;
    this->errorMessage = "null";
}
