//
// Created by ritam on 04/05/20.
//

#include "Pool.h"

Pool::Pool() = default;


Pool::Pool(Board board) {
    for (const auto &word : board.getWordList()) {
        for (const auto letter : word) {
            letters.push_back(letter);
        }
    }
    shuffle();
}

void Pool::shuffle() {
    std::random_device rd;
    std::shuffle(letters.begin(), letters.end(), rd);
}

void Pool::dealHand(unsigned tiles, Player &player) {
    std::vector<char> hand;
    std::cout << "1\n";
    for (size_t i = 0; i < tiles; i ++) {
        hand.push_back(letters.at(i));
    }
    letters.erase(letters.begin(), letters.begin() + tiles);
    player.setHand(hand);
}

std::vector<char> Pool::getContent() {
    return letters;
}
