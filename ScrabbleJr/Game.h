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

    Game(const std::vector<Player>& players, const Board& board, Pool pool);

    void nextPlayer();

    void makePlay();

    bool isFinished();

    void showLeaderboard();

    void showBoard();

    void showOthersHands();

    size_t getSize();

    void showAllHands();

private:

    std::vector<Player> _playerList;

    Board _board;

    Pool _pool;

    size_t _currentN{};

    Player _currentP;

    size_t _nPlayers{};

    size_t _WIDTH{};

    size_t _HEIGHT{};

    void exchangeTiles();

    void playTile();

    void showPlayerError();

    void giveHint();

};


#endif //PROGT2_GAME_H
