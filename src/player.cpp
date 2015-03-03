#include "player.h"

Player::Player(QObject *parent) : ReversiPlayer(parent)
{
    humanPlayer = true;
}

Player::~Player()
{
}

void Player::makeMove(BoardModel *model, Board::PlayerID playerId) {

}
