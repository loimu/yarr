#include "board.h"

Board::Board() : rows(8), cols(8)
{
    int oo1 = 16, oo2 = 32, oo3 = 48;
    dirFlags.insert(Board::N, (quint64) 0x0000 << oo3 | (quint64) 0x0000 << oo2
                    | (quint64) 0x0000 << oo1 | (quint64) 0x00ff);
    dirFlags.insert(Board::W, (quint64) 0x0101 << oo3 | (quint64) 0x0101 << oo2
                    | (quint64) 0x0101 << oo1 | (quint64) 0x0101);
    dirFlags.insert(Board::S, (quint64) 0xff00 << oo3 | (quint64) 0x0000 << oo2
                    | (quint64) 0x0000 << oo1 | (quint64) 0x0000);
    dirFlags.insert(Board::E, (quint64) 0x8080 << oo3 | (quint64) 0x8080 << oo2
                    | (quint64) 0x8080 << oo1 | (quint64) 0x8080);
    dirFlags.insert(Board::NE, dirFlags.value(Board::N) | dirFlags.value(Board::E));
    dirFlags.insert(Board::SE, dirFlags.value(Board::E) | dirFlags.value(Board::S));
    dirFlags.insert(Board::SW, dirFlags.value(Board::S) | dirFlags.value(Board::W));
    dirFlags.insert(Board::NW, dirFlags.value(Board::N) | dirFlags.value(Board::W));
    dirs << Board::N << Board::NE << Board::E << Board::SE << Board::S
         << Board::SW << Board::W << Board::NW;
    clear();
}

Board::~Board()
{
}

void Board::clear() {
    marks[0] = 0;
    marks[1] = 0;
}

void Board::init() {
    clear();
    placeBid(3, 4, Board::Black);
    placeBid(4, 3, Board::Black);
    placeBid(3, 3, Board::White);
    placeBid(4, 4, Board::White);
}

int Board::rowCount() const {
    return rows;
}

int Board::colCount() const {
    return cols;
}

quint64 Board::indexToMask(int row, int col) const {
    if(row < rows && col < cols)
        return Q_INT64_C(1) << (row * cols + col);
    return 0;
}

bool Board::placeBid(int row, int col, Board::PlayerID playerId) {
    quint64 mask = indexToMask(row, col);
    if(!(marks[!playerId] & mask)) {
        marks[playerId] |= mask;
        return true;
    }
    return false;
}

void Board::reverseBid(int row, int col) {
    quint64 mask = indexToMask(row, col);
    if((marks[0] ^ marks[1]) & mask) {
        marks[0] ^= mask;
        marks[1] ^= mask;
    }
}

int Board::getCell(int row, int col) const {
    quint64 mask = indexToMask(row, col);
    if(marks[0] & mask)
        return 0;
    if(marks[1] & mask)
        return 1;
    return -1;
}

void Board::checkCell(int row, int col) {
    quint64 currentCell = indexToMask(row, col);
    quint64 playerId = getCell(row, col);
    QVector<quint64> res = checkAllLines(currentCell, playerId);
    foreach(quint64 t, res) {
        if(marks[playerId] & t) {
            reverseLine(currentCell, t);
        }
    }
}

QVector<QPair<int, int> > Board::possibleMoves(bool playerId) {
    QVector<QPair<int, int> > res;
    QVector<quint64> resTemp;
    quint64 currentCell = 1;
    for(int i = 0; i < 64; i++, currentCell = currentCell << 1) {
        resTemp = checkAllLines(currentCell, playerId);
        foreach(quint64 t, resTemp) {
            if(!(marks[playerId] & t)) {
                res << maskToIndex(t);
            }
        }
    }
    return res;
}

quint64 Board::checkLine(quint64 mask, bool playerId, Board::Dir direction) {
    quint64 currentCell = mask, nextCell = getNextCell(currentCell, direction);
    if(marks[playerId] & nextCell) {
        while((marks[playerId] & nextCell) && (nextCell != currentCell)) {
            if(nextCell & dirFlags.value(direction)) {
                return mask;
            }
            currentCell = nextCell;
            nextCell = getNextCell(currentCell, direction);
        }
        return nextCell;
    }
    return currentCell;
}

QVector<quint64> Board::checkAllLines(quint64 currentCell, bool playerId) {
    QVector<quint64> res;
    quint64 nextCell;
    if(marks[playerId] & currentCell) {
        foreach(Board::Dir dir, dirs) {
            nextCell = checkLine(currentCell, !playerId, dir);
            if(nextCell != currentCell)
                res << nextCell;
        }
    }
    return res;
}

quint64 Board::getNextCell(quint64 currentCell, Board::Dir direction) {
    if(currentCell & dirFlags.value(direction))
        return currentCell;
    else
        return direction > 0 ? (currentCell << direction) : (currentCell >> -1 * direction);
}

void Board::reverseLine(quint64 startMask, quint64 endMask) {
    QPair<int, int> startIndex = maskToIndex(startMask);
    QPair<int, int> endIndex = maskToIndex(endMask);
    int diffColumn = endIndex.second - startIndex.second;
    int diffRow = endIndex.first - startIndex.first;
    if(!diffColumn || !diffRow || (qAbs(diffColumn) == qAbs(diffRow))) {
        int diffMax = qMax(qAbs(diffColumn), qAbs(diffRow));
        int dColumn = 0, dRow = 0;
        if(diffMax) {
            dColumn = diffColumn / diffMax;
            dRow = diffRow / diffMax;
        }
        for(int i = 1; i < diffMax; i++) {
            reverseBid(startIndex.first + dRow * i, startIndex.second + dColumn * i);
        }
    }
}

QPair<int, int> Board::maskToIndex(quint64 mask) const {
    int i = -1;
    while(mask) {
        mask = mask >> 1;
        i++;
    }
    if(i > -1)
        return QPair<int, int> (i / 8, i % 8);
    else
        return QPair<int, int> (-1, -1);
}

int Board::playerBidsCount(Board::PlayerID playerId) {
    quint64 mask = marks[playerId];
    int i = 0;
    while(mask) {
        if(mask & 1)
            i++;
        mask = mask >> 1;
    }
    return i;
}
