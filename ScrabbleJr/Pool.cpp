
#include "Pool.h"

Pool::Pool() = default;


Pool::Pool(Board board) {
    for (const char &i : board.getBoardLetters()) {
        letters.push_back(i);
    }
    shuffle();
}

void Pool::shuffle() {
    std::random_device rd;
    std::shuffle(letters.begin(), letters.end(), rd);
}

void Pool::dealHand(unsigned tiles, Player &player) {
    std::vector<char> hand = player.getHand();
    for (size_t i = 0; i < tiles; i++) {
        hand.push_back(letters.at(i));
    }

    letters.erase(letters.begin(), letters.begin() + tiles);

    player.setHand(hand);
}

std::vector<char> Pool::getContent() {
    return letters;
}

bool Pool::isEmpty() {
    return letters.size() != 0;
}
