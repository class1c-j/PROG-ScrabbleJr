//
// Created by ritam on 04/05/20.
//

#include "Player.h"
#include <iostream>

Player::Player() = default;


Player::Player(const std::string &name_) {
    name = name_;
}

void Player::setHand(const std::vector<char>& tiles) {
    for (const char i : tiles) {
        hand.push_back(i);
    }
}

void Player::showHand() {
    std::cout << name << "'s hand: ";
    for (const auto & i : hand) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

void Player::play(char letter, std::pair<char, char> coords, Board &board) {
    auto it = std::find(hand.begin(), hand.end(), letter);
    hand.erase(it);
    std::cout << "Deleted tiles";
    showHand();
    board.setPlayed(coords);
}

bool Player::isValidMove(char letter, std::pair<char, char> coords, Board board) {

    bool check = true;
    unsigned line = coords.first, col = coords.second;
    auto it = std::find(hand.begin(), hand.end(), letter);

    if (it == hand.end()) {
        check = false;
        std::cout << "Error Letter not in player's hand\n";
    } else if (board.getContent().at(line).at(col) != letter) {
        check = false;
        std::cout << "Error: letters do not correspond\n";
    } else if (board.isPlayed(coords)) {
        check = false;
        std::cout << "Error: place already played\n";
    } else {

        // TODO: needs fixing, disabled for now!!
        std::pair<char, char> firstNPlayedV;
        std::pair<char, char> firstNPlayedH;
        std::vector<std::pair<char, char> > starts = board.getWordsInPointStart(coords);
        std::cout << "Searched for words taht pass there\n";

        for (const auto &i : starts) {
            std::cout << "Found words starting on " << (int) i.first << ", " << (int) i.second << '\n';
        }

        std::pair<char, char> horizontal = starts.at(0);
        if (horizontal != std::pair<char, char>{-1, -1}) {
            for (size_t i = horizontal.second; i < board.getWord(horizontal, 'H').length(); i ++) {
                if (!board.isPlayed({line, i})) {
                    firstNPlayedH = {line, i};
                    std::cout << "First open space is " << line << ", " << i << "\n";
                    break;
                }
            }
        }

        std::pair<char, char> vertical = starts.at(1);
        if (vertical != std::pair<char, char>{-1, -1}) {
            for (size_t i = vertical.first; i < board.getWord(vertical, 'V').length(); i ++) {
                if (!board.isPlayed({i, col})) {
                    firstNPlayedV = {i, col};
                    std::cout << "First open space is " << i << ", " << col << '\n';
                    break;
                }
            }
        }

        std::cout << (coords == firstNPlayedH)  << (coords == firstNPlayedV);
        //if (!check) std::cout << "Not in the first free place\n";
    }
    return check;
}

std::string Player::getName() {
    return name;
}

void Player::incrementScore() {
    score ++;
}

unsigned Player::getScore() const {
    return score;
}

