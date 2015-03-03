#include "reversiplayer.h"

ReversiPlayer::ReversiPlayer(QObject *parent) : QObject(parent)
{
    boardModel = 0;
}

ReversiPlayer::~ReversiPlayer()
{
}

void ReversiPlayer::setModel(BoardModel *model) {
    boardModel = model;
}
