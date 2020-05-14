//
// Created by ritam on 05/05/20.
//

#ifndef PROGT2_GAME_H
#define PROGT2_GAME_H

#include "Player.h"
#include "../CommonFiles/Board.h"
#include "../CommonFiles/utility.h"
#include "Pool.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

class Game {
public:
    Game();

    Game(std::vector<Player> players, Board board, Pool pool);

    void nextPlayer();

    void makePlay();

    bool isFinished();

    void showLeaderboard();

    void showBoard();

    void showOthersHands();

    unsigned getSize();

    void showAllHands();

private:

    unsigned long _nPlayers{};

    unsigned _currentN{};

    std::vector<Player> _playerList;

    Board _board;

    Pool _pool;

    Player _currentP;

    void exchangeTiles();

    void playTile();

    void showMessage();

    void giveHint();

    int WIDTH;

    int HEIGHT;
};


#endif //PROGT2_GAME_H
