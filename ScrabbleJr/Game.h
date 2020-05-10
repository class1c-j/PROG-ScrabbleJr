//
// Created by ritam on 05/05/20.
//

#ifndef PROGT2_GAME_H
#define PROGT2_GAME_H

#include "Player.h"
#include "../CommonFiles/Board.h"
#include "../CommonFiles/utility.h"
#include "Pool.h"

class Game {
public:
    Game();

    Game(std::vector<Player> players, Board board, Pool pool);

    void nextPlayer();

    void makePlay();

    bool isFinished();

    Player getWinner();

    void makeTurn();

    void play();

private:
    unsigned _nPlayers{};
    unsigned _currentN{};
    std::vector<Player> _playerList;
    Board _board;
    Pool _pool;
    Player _currentP;
};


#endif //PROGT2_GAME_H
