//
// Created by ritam on 04/05/20.
//

#ifndef PROGT2_PLAYER_H
#define PROGT2_PLAYER_H

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "../CommonFiles/Board.h"
#include "gameIO.h"


class Player {
public:
    Player();

    explicit Player(const std::string &name_);

    void showHand();

    std::string getName() const;

    unsigned getScore() const;

    void incrementScore();

    void play(char letter, std::pair<char, char> coords, Board &board);

    bool isValidMove(char letter, std::pair<char, char> coords, Board board);

    std::vector<std::string> playableTiles(const Board &board);

    void removeTile(char tile);

    bool hasTile(char tile);

    void addTiles(const std::vector<char> &tiles);

    std::vector<char> getHand();

    std::vector<std::string> errors = {
            "ERROR: You do not have that tile in your hand.\n", "ERROR: Can't place that tile there.\n"
    };

    std::string error{};

    void setScore(int i);

private:
    std::string name;
    std::vector<char> hand;
    unsigned score{};
};


#endif //PROGT2_PLAYER_H
