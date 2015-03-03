#ifndef BOARD_H_
#define BOARD_H_

#include <QtGlobal>
#include <QPair>
#include <QVector>
#include <QHash>

class Board {

public:
    enum PlayerID {
        EMPTY = -1, Black = 0, White = 1
    };
    enum Dir {
        N = -8, NE = -7, E = 1, SE = 9, S = 8, SW = 7, W = -1, NW = -9
    };
    Board();
    virtual ~Board();
    void init();
    void clear();
    bool placeBid(int row, int col, Board::PlayerID playerId);
    int getCell(int row, int col) const;
    int rowCount() const;
    int colCount() const;
    void checkCell(int row, int col);
    QVector<QPair<int, int> > possibleMoves(bool playerId);
    int playerBidsCount(Board::PlayerID playerId);

private:
    int rows, cols;
    quint64 marks[2];
    QHash<Board::Dir, quint64> dirFlags;
    QVector<Board::Dir> dirs;
    quint64 indexToMask(int row, int col) const;
    QPair<int, int> maskToIndex(quint64 mask) const;
    void reverseLine(quint64 startMask, quint64 endMask);
    void reverseBid(int row, int col);
    quint64 checkLine(quint64 mask, bool playerId, Board::Dir direction);
    QVector<quint64> checkAllLines(quint64 number, bool playerId);
    quint64 getNextCell(quint64 currentCell, Board::Dir direction);
};

#endif /* BOARD_H_ */
