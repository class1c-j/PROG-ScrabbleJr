
#include "Player.h"
#include <iostream>

Player::Player() = default;


Player::Player(const std::string &name_) {
    name = name_;
}

void Player::setHand(const std::vector<char> &tiles) {
    hand = tiles;
}

void Player::addTiles(const std::vector<char> &tiles) {
    for (const auto &i : tiles) {
        std::cout << "adding " << i << " to player's tiles\n";
        hand.push_back(i);
    }
}

void Player::showHand() {
    std::cout << name << "'s hand: ";
    for (const auto &i : hand) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

void Player::play(char letter, std::pair<char, char> coords, Board &board) {
    auto it = std::find(hand.begin(), hand.end(), letter);
    hand.erase(it);  // take tile from player's hand
    board.setPlayed(coords);  // place the tile in the board
}

bool Player::isValidMove(char letter, std::pair<char, char> coords, Board board) {

    bool check;
    unsigned line = coords.first, col = coords.second;

    if (!hasTile(letter)) {
        check = false;
        //std::cout << "Error Letter not in player's hand\n";
    } else if (board.getContent().at(line).at(col) != letter) {
        check = false;
        //std::cout << "Error: letters do not correspond\n";
    } else if (board.isPlayed(coords)) {
        check = false;
        //std::cout << "Error: place already played\n";
    } else {

        std::pair<char, char> firstNPlayedV{};
        std::pair<char, char> firstNPlayedH{};
        std::vector<std::pair<char, char> > starts = board.getWordsInPointStart(coords);

        std::pair<char, char> horizontal = starts.at(0);
        if (horizontal != std::pair<char, char>{-1, -1}) {
            for (size_t i = horizontal.second; i < horizontal.second + board.getWord(horizontal, 'H').size(); i++) {
                if (!board.isPlayed({line, i})) {
                    firstNPlayedH = {line, i};
                    break;
                }
            }
        }

        std::pair<char, char> vertical = starts.at(1);
        if (vertical != std::pair<char, char>{-1, -1}) {
            for (size_t i = vertical.first; i < vertical.first + board.getWord(vertical, 'V').size(); i++) {
                if (!board.isPlayed({i, col})) {
                    firstNPlayedV = {i, col};
                    break;
                }
            }
        }

        check = (coords == firstNPlayedH) || (coords == firstNPlayedV);
        //if (!check) std::cout << "Not in the first free place\n";
    }
    return check;
}

std::string Player::getName() {
    return name;
}

void Player::incrementScore() {
    score++;
}

unsigned Player::getScore() const {
    return score;
}

std::vector<char> Player::playableTiles(const Board &board) {

    std::vector<char> playable;
    std::vector<std::pair<char, char>> coords;

    for (const auto &tile : hand) {
        for (int i = 0; i < board.getnLines(); i++) {
            for (int j = 0; j < board.getnCols(); j++) {
                std::pair<char, char> pair = {i, j};
                if (isValidMove(tile, {i, j}, board) &&
                    std::count(playable.begin(), playable.end(), tile) < std::count(hand.begin(), hand.end(), tile)
                    && std::count(coords.begin(), coords.end(), pair) == 0) {
                    playable.push_back(tile);
                    coords.emplace_back(i, j);
                }
            }
        }
    }

    return playable;
}

std::vector<char> Player::getHand() {
    return hand;
}

void Player::removeTile(char tile) {
    auto it = std::find(hand.begin(), hand.end(), tile);
    if (hasTile(tile)) {
        hand.erase(it);  // take tile from player's hand
    }
}

bool Player::hasTile (char tile) {
    auto it = std::find(hand.begin(), hand.end(), tile);
    return !(it == hand.end());
}
