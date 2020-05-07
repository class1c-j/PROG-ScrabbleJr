//
// Created by ritam on 05/05/20.
//

#ifndef PROGT2_GAME_H
#define PROGT2_GAME_H

#include "Player.h"
#include "../CommonFiles/Board.h"

class Game {
public:
    Game();
    explicit Game(const std::vector<Player>&);
    unsigned getCurrPlayerN() const;
    Player getCurrentPlayer();
    Player nextPlayer();


private:
    unsigned nPlayers{};
    unsigned currPlayerN{};
    std::vector<Player> playerList;
    Player currentPlayer{};
};


#endif //PROGT2_GAME_H
