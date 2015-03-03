#include <QtGui>
#include <QStateMachine>
#include <QPropertyAnimation>
#include <QSignalTransition>
#include <QTextCodec>

#include "tests.h"
#include "boardmodel.h"
#include "painter.h"
#include "player.h"
#include "solver.h"
#include "aboutdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    BoardTests t1;
    QTest::qExec(&t1);
    ui->setupUi(this);
    ui->mainToolBar->hide();
    // menu actions
    connect(ui->actionNew_Game, SIGNAL(triggered()), SLOT(startGame()));
    connect(ui->actionRules, SIGNAL(triggered()), SLOT(rules()));
    connect(ui->actionResign, SIGNAL(triggered()), SLOT(resign()));
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
    player = new Player(this);
    player->setModel(model);
    player->setPlayerId(Board::Black);
    connect(player, SIGNAL(moveMade()), SIGNAL(playerMadeMove()));
    computerPlayer = new Solver(this);
    computerPlayer->setModel(model);
    computerPlayer->setPlayerId(Board::White);
    connect(computerPlayer, SIGNAL(moveMade()), SIGNAL(playerMadeMove()));
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
    connect(playerFirstMoveState, SIGNAL(propertiesAssigned()), player, SLOT(takeTurn()));
    connect(playerSecondMoveState, SIGNAL(propertiesAssigned()), computerPlayer, SLOT(takeTurn()));
    connect(playerFirstMoveState, SIGNAL(propertiesAssigned()), SLOT(beforeMove()));
    connect(playerSecondMoveState, SIGNAL(propertiesAssigned()), SLOT(beforeMove()));
    connect(this, SIGNAL(playerMadeMove()), SLOT(afterMove()));
    inGameState->assignProperty(ui->actionResign, "enabled", true);
    outOfGameState->assignProperty(ui->actionResign, "enabled", false);
    machine->setInitialState(outOfGameState);
    machine->start();
}

void MainWindow::cellClicked(QModelIndex indexClicked) {
    if(this->property("isWaitingForHumanMove").toBool()) {
        if(model->makeMove(indexClicked, property("currentPlayerId").toInt()))
            playerMadeMove();
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
        nextMove();
    }
    nextMove();
}

void MainWindow::beforeMove() {
    QString count;
    count.append(tr("White:"));
    count.append((QString)model->playerBidsCount(player->getPlayerId()));
    count.append("/");
    count.append("Black:");
    count.append((QString)model->playerBidsCount(computerPlayer->getPlayerId()));
    ui->statusBar->showMessage(count);
}

void MainWindow::startGame() {
    this->statusBar()->showMessage(tr("New game is started"), 2000);
    model->init();
    setPlayers();
    gameStarted();
}

void MainWindow::resign() {
    gameFinished();
}

void MainWindow::checkGame() {
    int humanPlayerBidsCount = model->playerBidsCount(player->getPlayerId());
    int computerPlayerBidsCount = model->playerBidsCount(computerPlayer->getPlayerId());
    QString title, message;
    if(humanPlayerBidsCount > computerPlayerBidsCount) {
        title = tr("You won");
        message = tr("Congratulations!");
    } else if(humanPlayerBidsCount < computerPlayerBidsCount) {
        title = tr("You lost");
        message = tr("We're sorry.");
    } else if(humanPlayerBidsCount == computerPlayerBidsCount) {
        title = tr("There's a draw");
        message = tr("Not bad!");
    }
    QMessageBox::information(this, title, message);
    gameFinished();
}

void MainWindow::setPlayers() {
    player->setPlayerId(Board::White);
    computerPlayer->setPlayerId(Board::Black);
    inGameState->setInitialState(playerSecondMoveState);
    playerFirstMoveState->assignProperty(this, "isWaitingForHumanMove", true);
    playerFirstMoveState->assignProperty(this, "currentPlayerId", player->getPlayerId());
    playerSecondMoveState->assignProperty(this, "isWaitingForHumanMove", false);
    playerSecondMoveState->assignProperty(this, "currentPlayerId", computerPlayer->getPlayerId());
}

void MainWindow::about() {
    AboutDialog *dialog = new AboutDialog();
    dialog->show();
}

void MainWindow::rules() {
    QDialog *gameRules = new QDialog();
    gameRules->setWindowTitle("Game rules");
    gameRules->setFixedSize(500,600);
    gameRules->setModal(false);
    QPushButton *closeButton = new QPushButton(gameRules);
    closeButton->setText("Close");
    closeButton->setGeometry(435,570,60,25);
    closeButton->show();
    connect(closeButton, SIGNAL(clicked()), gameRules, SLOT(close()));
    QTextEdit * textEdit = new QTextEdit(gameRules);
    QFile rulesFile(":/rules.txt");
    rulesFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream ReadFile(&rulesFile);
    textEdit->setText(ReadFile.readAll());
    textEdit->setReadOnly(true);
    textEdit->setGeometry(5,5, 490,565);
    textEdit->show();
    gameRules->show();
}
