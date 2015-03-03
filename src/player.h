#ifndef PLAYER_H_
#define PLAYER_H_

#include "reversiplayer.h"

class Player : public ReversiPlayer {
    Q_OBJECT

public:
    Player(QObject *parent = 0);
    virtual ~Player();

public slots:
    void makeMove(BoardModel *model, Board::PlayerID playerId);
};

#endif /* PLAYER_H_ */
