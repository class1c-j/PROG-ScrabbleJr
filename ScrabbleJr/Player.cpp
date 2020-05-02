//
// Created by ritam on 01/05/20.
//

#include "Player.h"

Player::Player() = default;

Player::Player(const std::string &name_) {
    name = name_;
}

Move Player::getMove() {
    return move;
}


