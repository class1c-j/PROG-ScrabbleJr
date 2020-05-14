
#include "Pool.h"

Pool::Pool() = default;


Pool::Pool(Board board) {

    // get the _letters from the board used
    for (const char &i : board.getBoardLetters()) {
        _letters.push_back(i);
    }

    shuffle();
}

/**
 * @brief shuffles the pool in a non deterministic way
 */
void Pool::shuffle() {

    std::random_device rd;
    std::shuffle(_letters.begin(), _letters.end(), rd);
}


/**
 * @brief takes a number of tiles from the pool and gives them to a player
 * @param tiles - the number of tiles to be dealt
 * @param player - the player to whom the tiles are dealt
 */
void Pool::dealHand(unsigned tiles, Player &player) {

    std::vector<char> newTiles{};  // temporary vector to store the tiles to deal

    int taken = 0;  // keep track of how many tiles are taken from the pool, so that in limit situations
                    // the erase method does not try to erase more tiles than there are in the pool

    for (size_t i = 0; i < tiles; i++) {

        if (isEmpty()) break;

        newTiles.push_back(_letters.at(i));
        taken++;

    }

    _letters.erase(_letters.begin(), _letters.begin() + taken);  // delete the tiles from the pool

    player.addTiles(newTiles);  // add the tiles to player's hand
}


/**
 * @brief check if the pool is empty
 * @return true if the pool is empty, false otherwise
 */
bool Pool::isEmpty() {
    return _letters.empty();
}


/**
 * @brief Add a tile to the pool
 *
 * @param tile - the tile to be added
 */
void Pool::addTile(char tile) {

    _letters.push_back(tile);

    // the pool gets shuffled so that the tiles exchanged do not stay together
    shuffle();

}

/**
 * @brief get the amount of tiles present in the pool.
 * @return number of tiles
 */
size_t Pool::getSize() {
    return _letters.size();
}