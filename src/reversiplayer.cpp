#include "reversiplayer.h"

ReversiPlayer::ReversiPlayer(QObject *parent) : QObject(parent)
{
    boardModel = 0;
}

ReversiPlayer::~ReversiPlayer()
{
}

bool ReversiPlayer::isHumanPlayer() {
    return humanPlayer;
}

void ReversiPlayer::setPlayerId(Board::PlayerID playerId) {
    this->playerId = playerId;
}

Board::PlayerID ReversiPlayer::getPlayerId() {
    return playerId;
}

QString ReversiPlayer::getPlayerColor() {
    if(playerId == 1)
        return "White";
    else if(playerId == 0)
        return "Black";
    return "N/A";
}

void ReversiPlayer::setModel(BoardModel *model) {
    boardModel = model;
}

void ReversiPlayer::takeTurn() {
    boardModel->setCurrentPlayerId(playerId);
    if (boardModel->getPossibleMoves(playerId).count())
        makeMove(boardModel, playerId);
    else
        emit moveMade();
}
