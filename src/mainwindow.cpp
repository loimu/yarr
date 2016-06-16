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

#include <QtGui>
#include <QStateMachine>
#include <QPropertyAnimation>
#include <QSignalTransition>
#include <QTextCodec>
#include <QSettings>

#include "tests.h"
#include "boardmodel.h"
#include "painter.h"
#include "player.h"
#include "solver.h"
#include "settings.h"
#include "aboutdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->hide();
    // menu actions
    connect(ui->actionNew_Game, SIGNAL(triggered()), SLOT(startGame()));
    connect(ui->actionRules, SIGNAL(triggered()), SLOT(rules()));
    connect(ui->actionResign, SIGNAL(triggered()), SLOT(resign()));
    connect(ui->actionSettings, SIGNAL(triggered()), SLOT(settings()));
    connect(ui->actionAbout_YARR, SIGNAL(triggered()), SLOT(about()));
    model = new BoardModel(this);
    ui->tableView->setModel(model);
    ui->tableView->setFixedHeight(ui->tableView->horizontalHeader()->height() +
                                  ui->tableView->verticalHeader()->length() +2);
    ui->tableView->setFixedWidth(ui->tableView->verticalHeader()->width() +
                                 ui->tableView->horizontalHeader()->length()+2);
    Painter *painter = new Painter(ui->tableView);
    ui->tableView->setItemDelegate(painter);
    machine = new QStateMachine(this);
    outOfGameState = new QState(machine);
    inGameState = new QState(machine);
    playerFirstMoveState = new QState(inGameState);
    playerSecondMoveState = new QState(inGameState);
    checkBoardState = new QState(inGameState);
    firstPlayer = new Player(this);
    firstPlayer->setModel(model);
    firstPlayer->setPlayerId(Board::Black);
    connect(firstPlayer, SIGNAL(moveMade()), SIGNAL(playerMadeMove()));
    secondPlayer = new Solver(this);
    secondPlayer->setModel(model);
    secondPlayer->setPlayerId(Board::White);
    connect(secondPlayer, SIGNAL(moveMade()), SIGNAL(playerMadeMove()));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)),
            SLOT(cellClicked(QModelIndex)));
    initStateMachine();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initStateMachine() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "currentPlayerId");
    animation->setDuration(100);
    outOfGameState->addTransition(this, SIGNAL(gameStarted()), inGameState);
    inGameState->addTransition(this, SIGNAL(gameFinished()), outOfGameState);
    inGameState->addTransition(this, SIGNAL(gameStarted()), inGameState);
    playerFirstMoveState->addTransition(this, SIGNAL(nextMove()), playerSecondMoveState)->addAnimation(animation);
    playerSecondMoveState->addTransition(this, SIGNAL(nextMove()), playerFirstMoveState)->addAnimation(animation);
    connect(outOfGameState, SIGNAL(propertiesAssigned()), SLOT(outOfGame()));
    connect(playerFirstMoveState, SIGNAL(propertiesAssigned()), firstPlayer, SLOT(takeTurn()));
    connect(playerSecondMoveState, SIGNAL(propertiesAssigned()), secondPlayer, SLOT(takeTurn()));
    connect(this, SIGNAL(playerMadeMove()), SLOT(afterMove()));
    inGameState->assignProperty(ui->actionResign, "enabled", true);
    outOfGameState->assignProperty(ui->actionResign, "enabled", false);
    machine->setInitialState(outOfGameState);
    machine->start();
}

void MainWindow::cellClicked(QModelIndex indexClicked) {
    if(this->property("isWaitingForHumanMove").toBool()) {
        if(model->makeMove(indexClicked, property("currentPlayerId").toInt()))
            emit playerMadeMove();
    }
}

void MainWindow::outOfGame() {
    this->statusBar()->showMessage(tr("Press Ctrl+N to play"));
}

void MainWindow::afterMove() {
    if(model->getPossibleMoves(!this->property("currentPlayerId").toInt()).count() == 0) {
        if(model->getPossibleMoves(this->property("currentPlayerId").toInt()).count() == 0) {
            checkGame();
            return;
        }
        emit nextMove();
    }
    emit nextMove();
    updateCount();
}

void MainWindow::updateCount() {
    QString count;
    count.append(firstPlayer->getPlayerColor() + ":");
    count.append(QString::number(model->playerBidsCount(firstPlayer->getPlayerId())));
    count.append(" | ");
    count.append(secondPlayer->getPlayerColor() + ":");
    count.append(QString::number(model->playerBidsCount(secondPlayer->getPlayerId())));
    ui->statusBar->showMessage(count);
}

void MainWindow::startGame() {
    model->init();
    setPlayers();
    emit gameStarted();
}

void MainWindow::resign() {
    emit gameFinished();
    model->clear();
}

void MainWindow::checkGame() {
    int humanPlayerBidsCount = model->playerBidsCount(firstPlayer->getPlayerId());
    int computerPlayerBidsCount = model->playerBidsCount(secondPlayer->getPlayerId());
    QString title, message;
    if(humanPlayerBidsCount > computerPlayerBidsCount) {
        title = tr("You won");
        message = tr("Congratulations!");
    } else if(humanPlayerBidsCount < computerPlayerBidsCount) {
        title = tr("You lost");
        message = tr("We're sorry. Next time you'll be lucky!");
    } else if(humanPlayerBidsCount == computerPlayerBidsCount) {
        title = tr("It's a draw");
        message = tr("Not bad!");
    }
    QMessageBox::information(this, title, message);
    emit gameFinished();
}

void MainWindow::setPlayers() {
    QSettings settings("yarr", "YARR");
    if(settings.value("main/whiteEnabled").toBool()) {
        firstPlayer->setPlayerId(Board::White);
        secondPlayer->setPlayerId(Board::Black);
        inGameState->setInitialState(playerSecondMoveState);
    } else {
        firstPlayer->setPlayerId(Board::Black);
        secondPlayer->setPlayerId(Board::White);
        inGameState->setInitialState(playerFirstMoveState);
    }
    playerFirstMoveState->assignProperty(this, "isWaitingForHumanMove", true);
    playerFirstMoveState->assignProperty(this, "currentPlayerId", firstPlayer->getPlayerId());
    playerSecondMoveState->assignProperty(this, "isWaitingForHumanMove", false);
    playerSecondMoveState->assignProperty(this, "currentPlayerId", secondPlayer->getPlayerId());
}

void MainWindow::settings() {
    Settings *dialog = new Settings(this);
    dialog->show();
}

void MainWindow::rules() {
    QDialog *gameRules = new QDialog(this);
    gameRules->setWindowTitle(tr("Game rules"));
    gameRules->setFixedSize(500,600);
    gameRules->setModal(false);
    QPushButton *closeButton = new QPushButton(gameRules);
    closeButton->setText(tr("Close"));
    closeButton->setGeometry(435,570,60,25);
    closeButton->show();
    connect(closeButton, SIGNAL(clicked()), gameRules, SLOT(close()));
    QTextEdit *textEdit = new QTextEdit(gameRules);
    QFile rulesFile(":/rules.txt");
    rulesFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream ReadFile(&rulesFile);
    textEdit->setText(ReadFile.readAll());
    textEdit->setReadOnly(true);
    textEdit->setGeometry(5,5,490,565);
    textEdit->show();
    gameRules->show();
}

void MainWindow::about() {
    AboutDialog *dialog = new AboutDialog(this);
    dialog->show();
    ui->actionAbout_YARR->setEnabled(false);
    connect(dialog, SIGNAL(destroyed(bool)),
            ui->actionAbout_YARR, SLOT(setEnabled(bool)));
}
