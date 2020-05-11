
#include <algorithm>
#include "Game.h"

Game::Game() = default;

// TODO: MOVE THIS TO BETTER PLACE
bool operator<(const Player &player1, const Player &player2) {
    return player1.getScore() < player2.getScore();
}

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

            showBoard();
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

        showBoard();
        _currentP.showHand();

        possibleTiles = _currentP.playableTiles(_board).size();

        if (possibleTiles != 0) {  // if after playing one tile, another play becomes available, play normally

            while (!_currentP.isValidMove(tile, coords, _board)) {
                readLetter(tile);
                readCoordinates(coords, _board);
            }

            _currentP.play(tile, coords, _board);
            if (_board.finishedWord(coords)) _currentP.incrementScore();

            showBoard();
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

            showBoard();
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

void Game::giveHint() {

    std::vector<std::string> possible = _currentP.playableTiles(_board);
    std::cout << possible.front();

}

Player Game::getCurrentPlayer() {
    return _currentP;
}

void Game::showLeaderboard() {
    std::vector<Player> copy = _playerList;
    std::sort(copy.begin(), copy.end());
    std::cout << "NAME\tSCORE\n";
    for (const auto &player : copy) {
        std::cout << player.getName() << "\t" << player.getScore() << '\n';
    }
}

void Game::showBoard() {

    std::stringstream ss;
    std::stringstream toString;
    _board.showBoard(toString);
    std::string temp{};
    std::vector<std::string> lines{};
    std::string string = toString.str();
    ss << string;

    while (std::getline(ss, temp, '\n')) {
        lines.push_back(temp);
    }

    int l = 0;
    for (const auto &i : lines) {
        gotoxy(20, l);
        std::cout << i;
        l += 1;
    }

    std::cout << '\n';

}

void Game::showOthersHands() {

    const int WEIGHT = _board.getnLines() + 50;
    const int HEIGHT = _board.getnCols() + 3;

    int l = 5;
    std::vector<Player> notPlaying{};
    for (int i = 0; i < _nPlayers; i ++) {
        if (i != _currentN) notPlaying.push_back(_playerList.at(i));
    }
    while (notPlaying.size() != 3) {
        notPlaying.emplace_back();
    }

    gotoxy(2, 2);
    std::cout << notPlaying.at(0).getName();
    gotoxy(2, 3);
    std::cout << "Score: " << notPlaying.at(0).getScore();
    gotoxy(2, 4);
    notPlaying.at(0).showHand();

    gotoxy(WEIGHT, 2);
    std::cout << notPlaying.at(1).getName();
    gotoxy(WEIGHT, 3);
    std::cout << "Score: " << notPlaying.at(1).getScore();
    gotoxy(WEIGHT, 4);
    notPlaying.at(1).showHand();

    gotoxy(WEIGHT, HEIGHT - 5);
    std::cout << notPlaying.at(2).getName();
    gotoxy(WEIGHT, HEIGHT - 4);
    std::cout << "Score: " << notPlaying.at(2).getScore();
    gotoxy(WEIGHT, HEIGHT - 3);
    notPlaying.at(2).showHand();

    gotoxy(2, HEIGHT - 5);
    std::cout << "You";
    gotoxy(2, HEIGHT - 4);
    std::cout << "Score: " << _currentP.getScore();
    gotoxy(2, HEIGHT - 3);
    _currentP.showHand();


}

unsigned Game::getSize() {
    return _board.getnLines();
}




