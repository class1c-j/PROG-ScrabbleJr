
#include "Player.h"
#include <iostream>
#include <utility>

Player::Player() = default;


Player::Player(std::string name)
        : _name(std::move(name)) {
}


/**
 * @brief add some determined tiles to player's hand
 * @param tiles - vector that contains the tiles to be given to the player
 */
void Player::addTiles(const std::vector<char> &tiles) {

    for (const auto &i : tiles) {
        _hand.push_back(i);
    }

}


/**
 * @brief show the content of the player's hand
 */
void Player::showHand() {

    for (const auto &i : _hand) {
        std::cout << _tileColour << char(toupper(i)) << _noColour << ' ';
    }

}


/**
 * @brief places a piece from the player's hand on the board
 * @param letter - the tile to play
 * @param coords - the place on the board where the tile is to be placed
 * @param board - the board
 */
void Player::play(char letter, std::pair<char, char> coords, Board &board) {

    // take tile from player's hand
    auto it = std::find(_hand.begin(), _hand.end(), letter);
    _hand.erase(it);

    // place it on the board
    board.setPlayed(coords);
}


/**
 * @brief validates a move
 * @param letter - the tile chosen
 * @param coords - the place chosen
 * @param board  - the current board
 * @return true if the move can be made, false otherwise
 */
bool Player::isValidMove(char letter, std::pair<char, char> coords, Board board) {

    bool check;
    size_t col = coords.second;
    size_t line = coords.first;

    if (!hasTile(letter)) {

        check = false;
        error = errors.at(0);  // tell user he does not have the tile

    } else if (board.getContent().at(line).at(col) != letter || board.isPlayed(coords)) {

        check = false;
        error = errors.at(1);  // tell user that he can't play the selected tile on selected place

    } else {

        std::pair<char, char> firstNPlayedV = board.getFirstNotPlayedV(coords);
        std::pair<char, char> firstNPlayedH = board.getFirstNotPlayedH(coords);

        // check if the chosen place is the first free place for the words that pass there
        check = (coords == firstNPlayedH) || (coords == firstNPlayedV);

        if (!check) error = errors.at(1);  // tell user he can't play the selected tile on selected place
    }

    return check;
}


/**
 * @brief name of the player
 * @return name
 */
std::string Player::getName() const {
    return _name;
}


/**
 * @brief score of the player
 * @return score
 */
unsigned Player::getScore() const {
    return _score;
}


/**
 * @brief looks for the possible places where the player can play
 * @param board - the current board
 * @return vector with the coordinates where a tile can be placed by the player
 */
std::vector<std::pair<char, char> > Player::getPlayable(const Board &board) {

    std::vector<char> playable;
    std::vector<std::pair<char, char>> coords;  // temporary vector to store the places that have already been
    // accounted for, avoiding duplicates

    for (const auto &tile : _hand) {

        for (size_t i = 0; i < board.getnLines(); i++) {
            for (size_t j = 0; j < board.getnCols(); j++) {

                std::pair<char, char> pair = {i, j};

                // if a place is valid and it has not been counted yet
                if (isValidMove(tile, {i, j}, board) &&
                    std::count(playable.begin(), playable.end(), tile) < std::count(_hand.begin(), _hand.end(), tile)
                    && std::count(coords.begin(), coords.end(), pair) == 0) {

                    playable.push_back(tile);
                    coords.emplace_back(i, j);

                }

            }
        }

    }

    return coords;
}


/**
 * @brief generate hints that tell the player to look at a certain place where he can play
 * @param board - the current board
 * @return a vector with hints
 */
std::string Player::getHint(const Board &board) {

    std::pair<char, char> coords = getPlayable(board).front();
    char line = coords.first;
    char col = coords.second;

    std::stringstream hint;
    hint << "HINT: Do you have any tile to play on " << line + 'A' << col + 'a' << "? ";

    return hint.str();
}

/**
 * @brief
 * @param tile the tile to be taken from the player's hand
 */
void Player::removeTile(char tile) {

    // get the position of the tile in the hand
    auto it = std::find(_hand.begin(), _hand.end(), tile);

    if (hasTile(tile)) {

        _hand.erase(it);  // take it

    }

}


/**
 * @brief checks if the player has a given tile
 * @param tile - the tile to look for
 * @return true if the player has that tile, false otherwise
 */
bool Player::hasTile(char tile) {

    tile = tolower(tile);  // it doesn't make sense to have the tiles be case sensitive
    auto it = std::find(_hand.begin(), _hand.end(), tile);

    // if std::find does not find the tile, it returns the iterator _hand.end()
    return !(it == _hand.end());

}


/**
 * @brief get the tiles that the player has
 * @return the hand
 */
std::vector<char> Player::getHand() {
    return _hand;
}


/**
 * @brief add one to the player's score
 */
void Player::incrementScore() {
    _score++;
}


/**
 * @brief operator to compare two players based on their score
 * @param otherPlayer operand 2
 * @return true if operand 1 has a higher score than operand 2
 */
bool Player::operator>(const Player &otherPlayer) const {
    return this->_score > otherPlayer._score;
}
