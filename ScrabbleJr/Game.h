//
// Created by ritam on 05/05/20.
//

#ifndef PROGT2_GAME_H
#define PROGT2_GAME_H

#include "Player.h"
#include "../CommonFiles/Board.h"
#include "Pool.h"

class Game {
public:
    Game();

    Game(std::vector<Player> players, Board board, Pool pool);

    std::vector<Player> getPlayers();

    void setPlayers(std::vector<Player> players);

    Board getBoard();

    void setBoard(Board board);

    Pool getPool();

    void setPool(Pool pool);

    void setPlayer(Player player, unsigned id);

    unsigned nextPlayer();

private:
    unsigned nPlayers{};
    unsigned _currentN;
    std::vector<Player> _playerList;
    Board _board;
    Pool _pool;
};


#endif //PROGT2_GAME_H
