
#include <algorithm>
#include "Game.h"

Game::Game() = default;

// TODO: MOVE THIS TO BETTER PLACE
bool operator>(const Player &player1, const Player &player2) {
    return player1.getScore() > player2.getScore();
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

    // in case one player goes out of tiles, his turn is skipped
    while (_currentP.getHand().empty()) {
        std::cout << _currentP.getName() << " has no tiles. Passing\n";
        _currentN = (_currentN + 1) % _nPlayers;
        _currentP = _playerList.at(_currentN);
        std::cout << isFinished() << '\n';
    }

}

void Game::makeTurn() {

    makePlay();
    if (!isFinished()) nextPlayer();

}

void Game::makePlay() {

    size_t playable = _currentP.playableTiles(_board).size();

    if (playable == 0) {
        gotoxy(_board.getnCols() + 30, 15);
        std::cout << "\033[A\033[2K";
        printMessage(_currentP.getName() + ", Exchange 2 tiles", 0);
        if (_pool.isEmpty()) {
            std::cin.ignore();
        } else if (_pool.getSize() == 1) {
            exchangeTiles();
        } else {
            for (int i = 0; i < 2; i ++) exchangeTiles();
        }
    }

    playable = _currentP.playableTiles(_board).size();

    if (playable == 1) {
        gotoxy(_board.getnCols() + 28, 15);
        std::cout << "\033[A\033[2K";
        printMessage("It's your turn, " + _currentP.getName() + ", play 1 tile.", 0);
        playTile();
        _pool.dealHand(1, _currentP);
    } else if (playable >= 2) {
        gotoxy(_board.getnCols() + 28, 15);
        std::cout << "\033[A\033[2K";
        printMessage("It's your turn, " + _currentP.getName() + ", play 2 tiles.", 0);
        for (int i = 0; i < 2; i ++) playTile();
        _pool.dealHand(2, _currentP);
    } else {
        gotoxy(_board.getnCols() + 28, 15);
        std::cout << "\033[A\033[2K";
        printMessage("It's your turn, " +_currentP.getName() + ", press ENTER to pass.", 0);
        std::cin.ignore();
    }

    _playerList.at(_currentN) = _currentP;

}

void Game::exchangeTiles() {

    std::string input{};
    char tile{};

    while (!_currentP.hasTile(tile) || input.size() > 1) {

        showBoard();
        showAllHands();
        gotoxy(getSize() + 30, getSize() + 6);
        std::cout << "\033[A\033[2K";
        readLetter(input);
        tile = input.front();

        if (!_currentP.hasTile(tile)) {
            showLeaderboard();
        } else {
            break;
        }

    }

    _pool.dealHand(1, _currentP);
    _pool.addTile(tile);

    _playerList.at(_currentN) = _currentP;

}

void Game::playTile() {

    std::string input{};
    char tile{};
    std::pair<char, char> coords{};

    while (!_currentP.isValidMove(tile, coords, _board)) {

        showBoard();
        showAllHands();
        gotoxy(getSize() + 30, getSize() + 6);
        std::cout << "\033[A\033[2K";
        readLetter(input);

        while (input == "hint") {
            giveHint();
            showBoard();
            std::cin.ignore();
            gotoxy(getSize() + 30, getSize() + 6);
            std::cout << "\033[A\033[2K";
            readLetter(input);
        }

        readCoordinates(coords, _board);

        tile = input.at(0);

        if (!_currentP.isValidMove(tile, coords, _board)) {
            showMessage();
        } else {
            break;
        }

    }


    _currentP.play(tile, coords, _board);

    for (int i = 0; i < _board.finishedWord(coords); i ++) {
        _currentP.setScore(_currentP.getScore() + 1);
    }

    clearScreen();
    showBoard();
    showOthersHands();

    _playerList.at(_currentN) = _currentP;

}

bool Game::isFinished() {
    return _board.isFinished();
}

void Game::showLeaderboard() {

    clearScreen();
    std::vector<Player> copy = _playerList;
    std::sort(copy.begin(), copy.end(), std::greater<>());
    std::cout << "NAME\tSCORE\n";
    for (const auto &player : copy) {
        std::cout << player.getName() << "\t" << player.getScore() << '\n';
    }
    std::cout << "ENTER to go back to main menu ...";
    std::cin.ignore();

}

void Game::showBoard() {

    gotoxy(0, 0);
    _board.showBoard(std::cout);


}

void Game::showOthersHands() {

    const int WEIGHT = _board.getnLines() + 30;

    std::vector<Player> notPlaying{};
    for (int i = 0; i < _nPlayers; i ++) {
        if (i != _currentN) notPlaying.push_back(_playerList.at(i));
    }
    while (notPlaying.size() != 3) {
        notPlaying.emplace_back();
    }

    gotoxy(WEIGHT, 2);
    std::cout << notPlaying.at(0).getName();
    gotoxy(WEIGHT, 3);
    if (_nPlayers >= 2) std::cout << "Score: " << notPlaying.at(0).getScore();
    gotoxy(WEIGHT, 4);
    notPlaying.at(0).showHand();

    gotoxy(WEIGHT, 6);
    std::cout << notPlaying.at(1).getName();
    gotoxy(WEIGHT, 7);
    if (_nPlayers >= 3) std::cout << "Score: " << notPlaying.at(1).getScore();
    gotoxy(WEIGHT, 8);
    notPlaying.at(1).showHand();

    gotoxy(WEIGHT, 10);
    std::cout << notPlaying.at(2).getName();
    gotoxy(WEIGHT, 11);
    if (_nPlayers == 4) std::cout << "Score: " << notPlaying.at(2).getScore();
    gotoxy(WEIGHT, 12);
    notPlaying.at(2).showHand();

}

void Game::showAllHands() {

    const int HEIGHT = _board.getnCols() + 3;

    showOthersHands();

    gotoxy(2, HEIGHT + 1);
    std::cout << "You";
    gotoxy(2, HEIGHT + 2);
    std::cout << "Score: " << _currentP.getScore();
    gotoxy(2, HEIGHT + 3);
    _currentP.showHand();
}

unsigned Game::getSize() {
    return _board.getnLines();
}

void Game::showMessage() {

    gotoxy(_board.getnCols() + 28, 15);
    printMessage(_currentP.error, 1);
    _currentP.error = "";

}

void Game::giveHint() {

    gotoxy(_board.getnCols() + 28, 15);
    printMessage(_currentP.playableTiles(_board).front(), 3);

}