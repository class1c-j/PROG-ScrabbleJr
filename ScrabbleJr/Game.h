#ifndef PROGT2_GAME_H
#define PROGT2_GAME_H

#define _ENGINEERING "\033[48;2;140;45;25m"
#define _DEFAULT "\033[39;49m"

#include "Player.h"
#include "../CommonFiles/Board.h"
#include "../CommonFiles/utility.h"
#include "Pool.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <utility>

class Game {
public:

    // constructors

    Game();

    Game(const std::vector<Player>& players, const Board& board, Pool pool);

    // get

    size_t getSize();

    bool isFinished();

    // set and modify

    void nextPlayer();

    void makePlay();

    // show

    void showLeaderboard();

    void showBoard();

    void showAllPlayersInfo();

private:

    // get

    std::vector<Player> getWinners();

    // process

    void exchangeTile();

    void playTile();

    // show

    void showPlayerError();

    void showHint();

    // data

    std::vector<Player> m_playerList;

    Board m_board;

    Pool m_pool;

    size_t m_currentN{};

    Player m_currentP;

    size_t m_nPlayers{};

    size_t m_width{};

    size_t m_height{};


};


#endif //PROGT2_GAME_H
