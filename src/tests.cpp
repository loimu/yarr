#include "tests.h"

void BoardTests::init() {
    testBoard.clear();
}

void BoardTests::createEmptyBoard() {
    Q_ASSERT(testBoard.colCount() == 8);
    Q_ASSERT(testBoard.rowCount() == 8);
    Q_ASSERT(testBoard.playerBidsCount(Board::Black) == 0);
    Q_ASSERT(testBoard.playerBidsCount(Board::White) == 0);
}

void BoardTests::initBoard() {
    testBoard.init();
    Q_ASSERT(testBoard.playerBidsCount(Board::Black) == 2);
    Q_ASSERT(testBoard.playerBidsCount(Board::White) == 2);
    Q_ASSERT(testBoard.getCell(3,3) == Board::White);
    Q_ASSERT(testBoard.getCell(4,4) == Board::White);
    Q_ASSERT(testBoard.getCell(4,3) == Board::Black);
    Q_ASSERT(testBoard.getCell(3,4) == Board::Black);
}

void BoardTests::firstMove() {
    testBoard.init();
    testBoard.placeBid(3, 5, Board::White);
    testBoard.checkCell(3, 5);
    Q_ASSERT(testBoard.getCell(3,4) == Board::White);
}

void BoardTests::eightLinesCombo() {
    testBoard.placeBid(2, 2, Board::Black);
    testBoard.placeBid(2, 3, Board::Black);
    testBoard.placeBid(2, 4, Board::Black);
    testBoard.placeBid(3, 4, Board::Black);
    testBoard.placeBid(4, 4, Board::Black);
    testBoard.placeBid(4, 3, Board::Black);
    testBoard.placeBid(4, 2, Board::Black);
    testBoard.placeBid(3, 2, Board::Black);
    testBoard.placeBid(1, 1, Board::White);
    testBoard.placeBid(1, 3, Board::White);
    testBoard.placeBid(1, 5, Board::White);
    testBoard.placeBid(3, 5, Board::White);
    testBoard.placeBid(5, 5, Board::White);
    testBoard.placeBid(5, 3, Board::White);
    testBoard.placeBid(5, 1, Board::White);
    testBoard.placeBid(3, 1, Board::White);
    testBoard.placeBid(3, 3, Board::White);
    testBoard.checkCell(3, 3);
    Q_ASSERT(testBoard.playerBidsCount(Board::White) == 17);
    Q_ASSERT(testBoard.playerBidsCount(Board::Black) == 0);
}

void BoardTests::dottedLine() {
    testBoard.placeBid(3, 3, Board::Black);
    testBoard.placeBid(3, 4, Board::White);
    testBoard.placeBid(3, 5, Board::Black);
    testBoard.placeBid(3, 6, Board::White);
    testBoard.placeBid(3, 7, Board::Black);
    testBoard.checkCell(3, 7);
    Q_ASSERT(testBoard.getCell(3, 4) == Board::White);
    Q_ASSERT(testBoard.getCell(3, 6) == Board::Black);
}

void BoardTests::possibleMovesOnEmptyBoard() {
    Q_ASSERT(testBoard.possibleMoves(Board::White).count() == 0);
    Q_ASSERT(testBoard.possibleMoves(Board::Black).count() == 0);
}

void BoardTests::possibleMovesOnInitedBoard() {
    testBoard.init();
    QVector<QPair<int, int> > whitePossibleMoves = testBoard.possibleMoves(Board::White);
    Q_ASSERT(whitePossibleMoves.count() == 4);
    Q_ASSERT(whitePossibleMoves.contains(QPair<int, int>(3, 5)));
    Q_ASSERT(whitePossibleMoves.contains(QPair<int, int>(2, 4)));
    Q_ASSERT(whitePossibleMoves.contains(QPair<int, int>(5, 3)));
    Q_ASSERT(whitePossibleMoves.contains(QPair<int, int>(4, 2)));
    QVector<QPair<int, int> > blackPossibleMoves = testBoard.possibleMoves(Board::Black);
    Q_ASSERT(blackPossibleMoves.count() == 4);
    Q_ASSERT(blackPossibleMoves.contains(QPair<int, int>(2, 3)));
    Q_ASSERT(blackPossibleMoves.contains(QPair<int, int>(3, 2)));
    Q_ASSERT(blackPossibleMoves.contains(QPair<int, int>(4, 5)));
    Q_ASSERT(blackPossibleMoves.contains(QPair<int, int>(5, 4)));
}
