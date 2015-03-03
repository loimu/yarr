#include "boardmodel.h"

BoardModel::BoardModel(QObject *parent = 0) :
    QAbstractTableModel(parent), board()
{
    clear();
}

BoardModel::~BoardModel()
{
}

void BoardModel::init() {
    possibleMoves[0].clear();
    possibleMoves[1].clear();
    board.init();
    currentPlayerId = 0;
    lastMove = QModelIndex();
    refreshPossibleMoves();
    beginResetModel();
    endResetModel();
}

void BoardModel::clear() {
    possibleMoves[0].clear();
    possibleMoves[1].clear();
    board.clear();
    currentPlayerId = 0;
    lastMove = QModelIndex();
    beginResetModel();
    endResetModel();
}

int BoardModel::rowCount(const QModelIndex&) const {
    return board.rowCount();
}

int BoardModel::columnCount(const QModelIndex&) const {
    return board.colCount();
}

QVariant BoardModel::data(const QModelIndex& index, int role) const {
    if(role == Qt::DisplayRole || role == Qt::EditRole) {
        return board.getCell(index.row(), index.column());
    }
    if(role == Qt::BackgroundColorRole) {
        if (possibleMoves[currentPlayerId].contains(index)) {
            // green color
            return "#37a42c";
        }
        if(index == lastMove) {
            // dark green color
            return "#75511a";
        }
        // brown color
        return "#006e29";
    }
    return QVariant();
}

bool BoardModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(role == Qt::EditRole) {
        return board.placeBid(index.row(), index.column(), Board::PlayerID(value.toInt()));
    }
    return false;
}

bool BoardModel::makeMove(QModelIndex index, int playerId) {
    if(possibleMoves[playerId].contains(index) && board.placeBid(index.row(), index.column(), Board::PlayerID(playerId))) {
        board.checkCell(index.row(), index.column());
        lastMove = index;
        dataChanged(this->index(0, 0), this->index(7, 7));
        return true;
    }
    return false;
}

void BoardModel::refreshPossibleMoves() {
    for(int playerId = 0; playerId < 2; playerId++) {
        possibleMoves[playerId].clear();
        QVector<QPair<int, int> > pMoves = board.possibleMoves(playerId);
        for(int i = 0; i < pMoves.count(); i++) {
            possibleMoves[playerId].append(this->index(pMoves.at(i).first, pMoves.at(i).second));
        }
    }
}

QModelIndexList BoardModel::getPossibleMoves(int playerId) {
    if(playerId == 0 || playerId == 1) {
        return possibleMoves[playerId];
    }
    return QModelIndexList();
}

int BoardModel::playerBidsCount(int playerId) {
    return board.playerBidsCount(Board::PlayerID(playerId));
}

void BoardModel::setCurrentPlayerId(Board::PlayerID playerId) {
    currentPlayerId = playerId;
    refreshPossibleMoves();
    dataChanged(this->index(0, 0), this->index(7, 7));
}
