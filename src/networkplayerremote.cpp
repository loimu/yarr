/* ========================================================================
*    Copyright (C) 2014-2019 Blaze <blaze@vivaldi.net>
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

#include "networkplayerremote.h"

NetworkPlayerRemote::NetworkPlayerRemote(QObject *parent) : ReversiPlayer(parent)
{
    humanPlayer = false;
}

NetworkPlayerRemote::~NetworkPlayerRemote()
{
}

void NetworkPlayerRemote::makeMove(BoardModel *model, Board::PlayerID playerId){
    Q_UNUSED(playerId);
    // this method supposed to be receiving data from the remote server
    if(model) {
        //model->makeMove(QModelIndex(), playerId);
        emit moveMade();
    }
}
