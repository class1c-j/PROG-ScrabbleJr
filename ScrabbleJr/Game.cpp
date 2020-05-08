
#include <algorithm>
#include "Game.h"

Game::Game() = default;


Game::Game(std::vector<Player> players, Board board, Pool pool) {
    _playerList = std::move(players);
    _board = std::move(board);
    _pool = std::move(pool);
    _currentN = 0;
    nPlayers = _playerList.size();
}

std::vector<Player> Game::getPlayers() {
    return _playerList;
}

void Game::setPlayers(std::vector<Player> players) {
    _playerList = std::move(players);
}

Board Game::getBoard() {
    return _board;
}

void Game::setBoard(Board board) {
    _board = std::move(board);
}

Pool Game::getPool() {
    return _pool;
}

void Game::setPool(Pool pool) {
    _pool = pool;
}

void Game::setPlayer(Player player, unsigned int id) {
    _playerList.at(id) = std::move(player);
}

unsigned Game::nextPlayer() {
    return (_currentN + 1) % nPlayers;
}

