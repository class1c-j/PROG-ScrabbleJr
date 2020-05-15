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

    explicit Player(std::string name_);

    void showHand();

    std::string getName() const;

    unsigned getScore() const;

    void play(char letter, std::pair<char, char> coords, Board &board);

    bool isValidMove(char letter, std::pair<char, char> coords, Board board);

    std::vector<std::pair<char, char> > getPlayable(const Board &board);

    void removeTile(char tile);

    bool hasTile(char tile);

    void addTiles(const std::vector<char> &tiles);

    std::vector<char> getHand();


    std::string error{};

    void incrementScore();

    std::string getHint(const Board &board);

    bool operator >(const Player &otherPlayer) const;

private:

    std::string _name;

    std::vector<std::string> errors = {
            "ERROR: You do not have that tile in your hand.\n", "ERROR: Can't place that tile there.\n"
    };

    std::vector<char> _hand;

    unsigned _score{};

    std::string _tileColour = "\033[48;2;140;45;25m";

    std::string _noColour = "\033[39;49m";

};


#endif //PROGT2_PLAYER_H
