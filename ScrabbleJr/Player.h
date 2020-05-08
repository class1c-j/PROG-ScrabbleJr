//
// Created by ritam on 04/05/20.
//

#ifndef PROGT2_PLAYER_H
#define PROGT2_PLAYER_H

#include <vector>
#include <string>
#include <algorithm>
#include "../CommonFiles/Board.h"
#include "gameIO.h"

class Player {
public:
    Player();

    explicit Player(const std::string &name_);

    void setHand(const std::vector<char> &tiles);

    void showHand();

    std::vector<char> getHand();

    std::string getName();

    unsigned getScore() const;

    void incrementScore();

    void play(char letter, std::pair<char, char> coords, Board &board);

    bool isValidMove(char letter, std::pair<char, char> coords, Board board);

    std::vector<char> playableTiles(const Board &board);

private:
    std::string name;
    std::vector<char> hand;
    unsigned score{};
};


#endif //PROGT2_PLAYER_H
