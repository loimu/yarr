#ifndef REVERSIPLAYER_H_
#define REVERSIPLAYER_H_

#include <QObject>

#include "boardmodel.h"

class ReversiPlayer: public QObject {
    Q_OBJECT

    Board::PlayerID playerId;
    virtual void makeMove(BoardModel *model, Board::PlayerID playerId) = 0;

protected:
    bool humanPlayer;
    BoardModel *boardModel;

public:
    ReversiPlayer(QObject *parent = 0);
    virtual ~ReversiPlayer();
    bool isHumanPlayer();
    void setPlayerId(Board::PlayerID playerId);
    Board::PlayerID getPlayerId();
    QString getPlayerColor();
    void setModel(BoardModel *model);

public slots:
    void takeTurn();

signals:
    void moveMade();
};

#endif /* REVERSIPLAYER_H_ */
