#ifndef BOARDMODEL_H_
#define BOARDMODEL_H_

#include <QAbstractTableModel>

#include "board.h"

class BoardModel: public QAbstractTableModel {
    Q_OBJECT

    Board board;
    QModelIndexList possibleMoves[2];
    QModelIndex lastMove;
    void refreshPossibleMoves();
    int currentPlayerId;

public:
    explicit BoardModel(QObject* parent = nullptr);
    virtual ~BoardModel();
    void setCurrentPlayerId(Board::PlayerID playerId);
    QModelIndexList getPossibleMoves(int playerId);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool makeMove(QModelIndex index, int playerId);
    void init();
    void clear();
    int playerBidsCount(int playerId);
};

#endif /* BOARDMODEL_H_ */
