#include "solver.h"

Solver::Solver(QObject *parent) : ReversiPlayer(parent)
{
    humanPlayer = false;
}

Solver::~Solver()
{
}

void Solver::makeMove(BoardModel *model, Board::PlayerID playerId) {
    if(model) {
        if(model->getPossibleMoves(playerId).count()) {
            model->makeMove(model->getPossibleMoves(playerId).first(), playerId);
            moveMade();
        }
    }
}
