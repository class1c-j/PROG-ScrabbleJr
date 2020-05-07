//
// Created by ritam on 04/05/20.
//

#ifndef PROGT2_POOL_H
#define PROGT2_POOL_H

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "../CommonFiles/utility.h"
#include "../CommonFiles/Board.h"
#include "Player.h"

class Pool {
public:
    Pool();
    explicit Pool(Board board);
    void dealHand(unsigned tiles, Player &player);
    std::vector<char> getContent();
private:
    void shuffle();
    std::vector<char> letters;
};


#endif //PROGT2_POOL_H
