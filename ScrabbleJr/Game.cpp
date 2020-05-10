
#include <algorithm>
#include "Game.h"

Game::Game() = default;


Game::Game(std::vector<Player> players, Board board, Pool pool) {
    _playerList = std::move(players);
    _board = std::move(board);
    _pool = std::move(pool);
    _currentN = 0;
    _currentP = _playerList.at(_currentN);
    _nPlayers = _playerList.size();
}


void Game::nextPlayer() {
    _currentN = (_currentN + 1) % _nPlayers;
    _currentP = _playerList.at(_currentN);
}

void Game::makeTurn() {
    makePlay();
    nextPlayer();
}

void Game::makePlay() {

    _board.showBoard(std::cout);
    _currentP.showHand();

    char tile{};
    std::pair<char, char> coords{};

    unsigned possibleTiles = _currentP.playableTiles(_board).size();

    if (possibleTiles >= 2) {

        for (int i = 0; i < 2; i ++) {

            while (!_currentP.isValidMove(tile, coords, _board)) {
                readLetter(tile);
                readCoordinates(coords, _board);
            }

            _currentP.play(tile, coords, _board);
            if (_board.finishedWord(coords)) _currentP.incrementScore();

            _board.showBoard(std::cout);
            _currentP.showHand();
        }

        if (!_pool.isEmpty()) _pool.dealHand(2, _currentP);
        _currentP.showHand();

    } else if (possibleTiles == 1) {


        while (!_currentP.isValidMove(tile, coords, _board)) {
            readLetter(tile);
            readCoordinates(coords, _board);
        }

        _currentP.play(tile, coords, _board);
        if (_board.finishedWord(coords)) _currentP.incrementScore();

        _board.showBoard(std::cout);
        _currentP.showHand();

        possibleTiles = _currentP.playableTiles(_board).size();

        if (possibleTiles != 0) {  // if after playing one tile, another play becomes available, play normally

            while (!_currentP.isValidMove(tile, coords, _board)) {
                readLetter(tile);
                readCoordinates(coords, _board);
            }

            _currentP.play(tile, coords, _board);
            if (_board.finishedWord(coords)) _currentP.incrementScore();

            _board.showBoard(std::cout);
            _currentP.showHand();

            if (!_pool.isEmpty()) _pool.dealHand(2, _currentP);
            _currentP.showHand();

        } else {  // else, draw only one tile from the pool

            if (!_pool.isEmpty()) _pool.dealHand(1, _currentP);
            _currentP.showHand();

        }

    } else {

        std::cout << "None of your tiles can be played. Choose 2 to exchange for new ones\n";

        for (int i = 0; i < 2; i ++) {

            readLetter(tile);
            while (true) {
                if (!_currentP.hasTile(tile)) {
                    readLetter(tile);
                } else {
                    tile = '0';
                    break;
                }
            }

            _currentP.removeTile(tile);
            _pool.addTile(tile);

            _board.showBoard(std::cout);
            _currentP.showHand();
        }
        if (!_pool.isEmpty()) _pool.dealHand(2, _currentP);

        if (possibleTiles != 0) {  // if the newly dealt tiles allow the player to play, play
            makePlay();
        }
    }

    _playerList.at(_currentN) = _currentP;
}

bool Game::isFinished() {
    return _board.isFinished();
}

Player Game::getWinner() {

    std::sort(_playerList.begin(), _playerList.end());
    return _playerList.front();
}

void Game::play() {
    while (!isFinished()) {
        makeTurn();
    }
}


