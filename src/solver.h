#ifndef SOLVER_H_
#define SOLVER_H_

#include "reversiplayer.h"

class Solver : public ReversiPlayer {
    Q_OBJECT

    void makeMove(BoardModel *model, Board::PlayerID playerId);

public:
    Solver(QObject *parent = 0);
    virtual ~Solver();
};

#endif /* SOLVER_H_ */
