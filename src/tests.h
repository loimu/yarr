#ifndef TESTS_H_
#define TESTS_H_

#include <QtTest/QtTest>
#include <QObject>

#include "board.h"

class BoardTests: public QObject {
    Q_OBJECT

    Board testBoard;

public:
    BoardTests(QObject *parent = 0) : QObject(parent) {
    }

private slots:
    void init();
    void createEmptyBoard();
    void initBoard();
    void firstMove();
    void eightLinesCombo();
    void dottedLine();
    void possibleMovesOnEmptyBoard();
    void possibleMovesOnInitedBoard();
};

#endif /* TESTS_H_ */
