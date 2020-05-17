
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

    // constructors

    Pool();

    explicit Pool(Board board);

    // gets

    bool isEmpty();

    size_t getSize();

    // sets and methods that change the object

    void dealHand(unsigned tiles, Player &player);

    void addTile(char tile);


private:

    void shuffle();

    std::vector<char> m_letters;
};


#endif //PROGT2_POOL_H
