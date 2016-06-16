/* ========================================================================
*    Copyright (C) 2014-2016 Blaze <blaze@vivaldi.net>
*
*    This file is part of YARR.
*
*    YARR is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    YARR is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with YARR.  If not, see <http://www.gnu.org/licenses/>.
* ======================================================================== */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if QT_VERSION >= 0x050000
    #include <QtWidgets>
#endif

#include <QMainWindow>
#include <QModelIndex>

class QStateMachine;
class QState;
class BoardModel;
class ReversiPlayer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    BoardModel *model;
    ReversiPlayer *firstPlayer, *secondPlayer;
    QStateMachine *machine;
    QState *outOfGameState, *inGameState, *checkBoardState,
           *playerFirstMoveState, *playerSecondMoveState;
    void initStateMachine();
    void setPlayers();

signals:
    void playerMadeMove();
    void nextMove();
    void gameFinished();
    void gameStarted();

private slots:
    void cellClicked(QModelIndex index);
    void outOfGame();
    void checkGame();
    void afterMove();
    void updateCount();
    // menu slots
    // menu Game
    void startGame();
    void resign();
    // menu Preferences
    void settings();
    // menu Help
    void rules();
    void about();
};

#endif // MAINWINDOW_H
