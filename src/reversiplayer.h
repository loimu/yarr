#ifndef REVERSIPLAYER_H_
#define REVERSIPLAYER_H_

#include <QtCore/QObject>
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
    bool isHumanPlayer() {
        return humanPlayer;
    }
    void setPlayerId(Board::PlayerID playerId) {
        this->playerId = playerId;
    }
    Board::PlayerID getPlayerId() const {
        return playerId;
    }
    void setModel(BoardModel *model);

public slots:
    void takeTurn() {
        boardModel->setCurrentPlayerId(playerId);
        if (boardModel->getPossibleMoves(playerId).count()) {
            makeMove(boardModel, playerId);
        } else {
            moveMade();
        }
    }

signals:
    void moveMade();
};

#endif /* REVERSIPLAYER_H_ */
