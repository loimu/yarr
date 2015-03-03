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
