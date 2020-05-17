
#ifndef PROGT2_PLAYER_H
#define PROGT2_PLAYER_H

#define _ENGINEERING "\033[48;2;140;45;25m"
#define _DEFAULT "\033[39;49m"

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "../CommonFiles/Board.h"
#include "gameIO.h"


class Player {

public:

    // constructors

    Player();

    explicit Player(std::string name);

    // get

    std::string getName() const;

    unsigned getScore() const;

    std::vector<char> getHand();

    std::vector<std::pair<char, char> > getPlayable(const Board &board);

    std::string getHint(const Board &board);

    // set or methods that modify the object

    void addTiles(const std::vector<char> &tiles);

    void removeTile(char tile);

    bool hasTile(char tile);

    void incrementScore();

    // processing and others

    bool isValidMove(char letter, std::pair<char, char> coords, Board board);

    void play(char letter, std::pair<char, char> coords, Board &board);

    // show

    void showHand();

    // data

    std::string error{};

    // operator

    bool operator >(const Player &otherPlayer) const;

private:

    std::string m_name;

    std::vector<std::string> m_errors = {
            "ERROR: You do not have that tile in your hand.\n", "ERROR: Can't place that tile there.\n"
    };

    std::vector<char> m_hand;

    unsigned m_score{};

};


#endif //PROGT2_PLAYER_H
