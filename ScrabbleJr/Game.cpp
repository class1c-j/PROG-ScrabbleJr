
#include <iostream>
#include "Game.h"

Game::Game() = default;

Game::Game(const std::vector<Player> &players) {
    nPlayers = players.size();
    currentPlayer = players.at(0);
    playerList = players;
}

unsigned Game::getCurrPlayerN() const {
    return currPlayerN;
}


Player Game::nextPlayer() {
    currentPlayer = playerList.at((currPlayerN + 1) % nPlayers);
    std::cout << currentPlayer.getName();
    return currentPlayer;
}

Player Game::getCurrentPlayer() {
    return playerList.at(currPlayerN);
}
