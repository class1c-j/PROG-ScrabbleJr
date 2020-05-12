
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
    while (_currentP.getHand().empty()) {
        std::cout << _currentP.getName() << " has no tiles. Passing\n";
        _currentN = (_currentN + 1) % _nPlayers;
        _currentP = _playerList.at(_currentN);
    }
}

void Game::makeTurn() {

    makePlay();
    nextPlayer();
}

void Game::makePlay() {

    size_t playable = _currentP.playableTiles(_board).size();

    if (playable == 0) {
        gotoxy(_board.getnCols() + 30, 15);
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
        gotoxy(_board.getnCols() + 30, 15);
        printMessage(_currentP.getName() + ", Play 1 tiles", 0);
        playTile();
        _pool.dealHand(1, _currentP);
    } else if (playable >= 2) {
        gotoxy(_board.getnCols() + 30, 15);
        printMessage(_currentP.getName() + ", Play 2 tiles", 0);
        for (int i = 0; i < 2; i ++) playTile();
        _pool.dealHand(2, _currentP);
    } else {
        printMessage(_currentP.getName() + ", Press ENTER to pass", 0);
        std::cin.ignore();
    }

    _playerList.at(_currentN) = _currentP;

}

void Game::exchangeTiles() {

    char tile{};

    showBoard();
    showOthersHands();
    gotoxy(2, _board.getnLines() + 6);
    std::cout << "Your tiles\n";
    gotoxy(2, getSize() + 7);
    _currentP.showHand();
    std::cout << '\n';
    gotoxy(_board.getnCols() + 30, _board.getnLines() + 5);
    std::cout << "\033[A\033[2K";
    readLetter(tile);

    while (!_currentP.hasTile(tile)) {

        clearScreen();
        showBoard();
        showOthersHands();
        showMessage();
        gotoxy(2, _board.getnLines() + 6);
        std::cout << "Your tiles\n";
        gotoxy(2, getSize() + 7);
        _currentP.showHand();
        std::cout << '\n';
        gotoxy(_board.getnCols() + 30, _board.getnLines() + 5);
        std::cout << "\033[A\033[2K";
        readLetter(tile);

    }

    _pool.dealHand(1, _currentP);
    _pool.addTile(tile);

    _playerList.at(_currentN) = _currentP;

}

void Game::playTile() {


    char tile = 0;
    std::pair<char, char> coords{};

    showBoard();
    showOthersHands();
    gotoxy(2, getSize() + 6);
    std::cout << "Your tiles: ";
    gotoxy(2, getSize() + 7);
    _currentP.showHand();
    std::cout << '\n';
    gotoxy(_board.getnCols() + 30, getSize() + 5);
    std::thread hint([this, tile] () {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        if (tile == 0) {
            giveHint();
        }
    });
    hint.detach();
    readLetter(tile);
    gotoxy(2, _board.getnLines() + 6);
    std::cout << "Your tiles: ";  // clearing the line causes this to disappear
    readCoordinates(coords, _board);

    while (!_currentP.isValidMove(tile, coords, _board)) {

        clearScreen();
        showBoard();
        showOthersHands();
        showMessage();
        tile = 0;
        gotoxy(2, getSize() + 6);
        std::cout << "Your tiles: ";
        gotoxy(2, getSize() + 7);
        _currentP.showHand();
        std::cout << '\n';
        std::thread hint([this, tile] () {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            if (tile == 0) {
                giveHint();
            }
        });
        hint.detach();
        gotoxy(_board.getnCols() + 30, getSize() + 5);
        readLetter(tile);
        gotoxy(2, _board.getnLines() + 6);
        std::cout << "Your tiles: ";  // clearing the line causes this to disappear
        readCoordinates(coords, _board);

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
    gotoxy(2, getSize() + 6);
    std::cout << "Your tiles: ";
    gotoxy(2, getSize() + 7);
    _currentP.showHand();

    _playerList.at(_currentN) = _currentP;

}

bool Game::isFinished() {
    return _board.isFinished();
}

Player Game::getWinner() {

    std::sort(_playerList.begin(), _playerList.end());
    return _playerList.front();
}


void Game::giveHint() {

    std::vector<std::string> possible = _currentP.playableTiles(_board);
    gotoxy(_board.getnCols() + 30, getSize() + 5);
    printMessage(possible.front(), 0);
    gotoxy(2, _board.getnLines() + 6);
    std::cout << "Your tiles: ";  // clearing the line causes this to disappear
    gotoxy(_board.getnCols() + 50, _board.getnLines() + 5);
    std::cout << ": ";
    std::cin.ignore();


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

    gotoxy(0, 0);
    _board.showBoard(std::cout);


}

void Game::showOthersHands() {

    const int WEIGHT = _board.getnLines() + 30;
    const int HEIGHT = _board.getnCols() + 3;

    int l = 5;
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
    if (!notPlaying.at(0).getName().empty()) std::cout << "Score: " << notPlaying.at(0).getScore();
    gotoxy(WEIGHT, 4);
    notPlaying.at(0).showHand();

    gotoxy(WEIGHT, 6);
    std::cout << notPlaying.at(1).getName();
    gotoxy(WEIGHT, 7);
    if (!notPlaying.at(1).getName().empty()) std::cout << "Score: " << notPlaying.at(1).getScore();
    gotoxy(WEIGHT, 8);
    notPlaying.at(1).showHand();

    gotoxy(WEIGHT, 10);
    std::cout << notPlaying.at(2).getName();
    gotoxy(WEIGHT, 11);
    if (!notPlaying.at(2).getName().empty()) std::cout << "Score: " << notPlaying.at(2).getScore();
    gotoxy(WEIGHT, 12);
    notPlaying.at(2).showHand();

}

void Game::showAllHands() {

    const int WEIGHT = _board.getnLines() + 30;
    const int HEIGHT = _board.getnCols() + 3;

    int l = 5;
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
    if (!notPlaying.at(0).getName().empty()) std::cout << "Score: " << notPlaying.at(0).getScore();
    gotoxy(WEIGHT, 4);
    notPlaying.at(0).showHand();

    gotoxy(WEIGHT, 6);
    std::cout << notPlaying.at(1).getName();
    gotoxy(WEIGHT, 7);
    if (!notPlaying.at(1).getName().empty()) std::cout << "Score: " << notPlaying.at(1).getScore();
    gotoxy(WEIGHT, 8);
    notPlaying.at(1).showHand();

    gotoxy(WEIGHT, 10);
    std::cout << notPlaying.at(2).getName();
    gotoxy(WEIGHT, 11);
    if (!notPlaying.at(2).getName().empty()) std::cout << "Score: " << notPlaying.at(2).getScore();
    gotoxy(WEIGHT, 12);
    notPlaying.at(2).showHand();

    gotoxy(2, HEIGHT + 2);
    std::cout << "You";
    gotoxy(2, HEIGHT + 3);
    std::cout << "Score: " << _currentP.getScore();
    gotoxy(2, HEIGHT + 4);
    _currentP.showHand();
}

unsigned Game::getSize() {
    return _board.getnLines();
}

void Game::showMessage() {

    gotoxy(_board.getnCols() + 30, 15);
    printMessage(_currentP.error, 1);
    _currentP.error = "";

}




