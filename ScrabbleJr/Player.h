//
// Created by ritam on 01/05/20.
//

#ifndef PROGT2_PLAYER_H
#define PROGT2_PLAYER_H

#include <string>
#include <vector>
#include "../BoardBuilder/editIO.h"
#include "../CommonFiles/Board.h"

struct Move {std::pair<char, char>coords{}; char dir{};};

class Player {
public:
    Player();
    explicit Player(const std::string &name_);
    Move getMove();
    void play();
private:
    std::string name;
    std::vector<char> hand;
    Move move;
};


#endif //PROGT2_PLAYER_H
