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
    void beforeMove();
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
