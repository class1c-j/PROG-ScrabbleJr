
#include "Game.h"

#include <utility>

Game::Game() = default;

Game::Game(const std::vector<Player>& players, const Board& board, Pool pool)

        : _playerList(players), _board(board), _pool(std::move(pool)), _currentN(0),
        _currentP(_playerList.at(_currentN)), _nPlayers(players.size()), WIDTH(board.getnCols() + 30),
        HEIGHT(board.getnLines() + 3) {

}


/**
 * @brief calculates and sets the next player to play
 */
void Game::nextPlayer() {

    _currentN = (_currentN + 1) % _nPlayers;
    _currentP = _playerList.at(_currentN);

    // in case one player goes out of tiles, his turn is skipped
    while (_currentP.getHand().empty() && !isFinished()) {

        goToXY(WIDTH, 15);
        clearLineAndGoUp();
        printMessage(_currentP.getName() + " has no tiles. Press ENTER to pass ...", 0);

        _currentN = (_currentN + 1) % _nPlayers;
        _currentP = _playerList.at(_currentN);

    }

}


/**
 * @brief makes a play taking into consideration how many tiles can be played and updates the player state
 */
void Game::makePlay() {

    size_t playable = _currentP.getPlayable(_board).size();  // count with the possible plays


    if (playable == 0) {

        goToXY(WIDTH, 15);
        clearLineAndGoUp();

        if (!_pool.isEmpty()) {

            if (_pool.getSize() == 1) {  // if there is only one tile in the pool, exchange it

                printMessage(_currentP.getName() + ", Exchange 2 tiles", 0);
                exchangeTiles();

            } else {  // exchange 2 tiles

                for (int i = 0; i < 2; i++) {

                    printMessage(_currentP.getName() + ", Exchange 2 tiles", 0);
                    exchangeTiles();

                }
            }

        }
    }

    // after the exchange, the player might be able to play, so the count is updated
    playable = _currentP.getPlayable(_board).size();

    if (playable == 1) {

        goToXY(WIDTH, 15);
        clearLineAndGoUp();

        printMessage("It's your turn, " + _currentP.getName() + ", play 1 tile.", 0);

        // place 1 tile, take 1 tile
        playTile();
        _pool.dealHand(1, _currentP);

    } else if (playable >= 2) {


        // place 2 tiles, take 2 tiles
        for (int i = 0; i < 2; i++) {

            goToXY(WIDTH, 15);
            clearLineAndGoUp();

            printMessage("It's your turn, " + _currentP.getName() + ", play 2 tiles.", 0);
            playTile();

        }

        _pool.dealHand(2, _currentP);

    } else {

        goToXY(WIDTH, 15);
        clearLineAndGoUp();

        // There are no possible plays, the player must pass
        printMessage("It's your turn, " + _currentP.getName() + ", press ENTER to pass.", 0);
        std::cin.ignore();

    }

    // update player state
    _playerList.at(_currentN) = _currentP;

}


/**
 * @brief exchanges a player's tile for a new one in case he has no possible plays and the pool is not empty
 * and updates the player state
 */
void Game::exchangeTiles() {

    std::string input{};
    char tile{};

    showBoard();
    showAllHands();

    while (!_currentP.hasTile(tile) || input.size() > 1) {

        goToXY(WIDTH, HEIGHT + 3);
        clearLineAndGoUp();  // TODO: SYSTEM AGNOSTIC

        readLetter(input);
        while (input == "hint") {

            readLetter(input); // hints are not allowed in this phase

        }

        tile = input.front();

        if (!_currentP.hasTile(tile)) {

            // FIXME: aaaaaaa
            printMessage("You do not have that tile", 1);

        } else {

            break;

        }

    }

    _currentP.removeTile(tile);  // take the tile from the player
    _pool.addTile(tile);        // add the tile to the pool

    _pool.dealHand(1, _currentP);  // give a new tile to the player

    clearScreen();
    showBoard();
    showAllHands();

    // update player state
    _playerList.at(_currentN) = _currentP;

}


/**
 * @brief plays a tile
 */
void Game::playTile() {

    std::string input{};
    char tile{};
    std::pair<char, char> coords{};

    while (!_currentP.isValidMove(tile, coords, _board)) {

        showBoard();
        showAllHands();

        goToXY(WIDTH, getSize() + 6);
        clearLineAndGoUp();

        readLetter(input);

        while (input == "hint") {

            giveHint();

            goToXY(WIDTH, getSize() + 6);
            clearLineAndGoUp();

            readLetter(input);

        }

        readCoordinates(coords, _board);

        tile = input.at(0);

        if (!_currentP.isValidMove(tile, coords, _board)) {
            showPlayerError();  // FIXME make this better
        } else {
            break;
        }

    }


    _currentP.play(tile, coords, _board);

    // check if the played scored
    for (int i = 0; i < _board.finishedWord(coords); i++) {
        _currentP.incrementScore();
    }


    clearScreen();
    showBoard();
    showAllHands();

    // update player state
    _playerList.at(_currentN) = _currentP;

}


/**
 * @brief check is the game is finished
 * @return true if the game has ended, false otherwise
 */
bool Game::isFinished() {
    return _board.isFinished();
}


/**
 * @brief show a leaderboard and som ascii art as an end screen after the board is finished
 */
void Game::showLeaderboard() {

    clearScreen();
    std::sort(_playerList.begin(), _playerList.end(), std::greater<>());

    std::cout << " __| |____________________________________________| |__\n"
                 "(__   ____________________________________________   __)\n"
                 "   | |                                            | |\n"
                 "   | |               BOARD FINISHED               | |\n"
                 "   | |                                            | |\n";
    std::cout << "   | |  ";

    if (_nPlayers >= 1)
        std::cout << std::setw(37) << std::left << _playerList.at(0).getName() << std::setfill('0') << std::right
                  << std::setw(2)
                  << _playerList.at(0).getScore() << std::setfill(' ');

    else std::cout << std::setw(39) << " ";

    std::cout << "   | |\n";
    std::cout << "   | |  ";

    if (_nPlayers >= 2)
        std::cout << std::setw(37) << std::left << _playerList.at(1).getName() << std::setfill('0') << std::right
                  << std::setw(2)
                  << _playerList.at(1).getScore() << std::setfill(' ');

    else std::cout << std::setw(39) << " ";

    std::cout << "   | |\n";
    std::cout << "   | |  ";

    if (_nPlayers >= 3)
        std::cout << std::setw(37) << std::left << _playerList.at(2).getName() << std::setfill('0') << std::right
                  << std::setw(2)
                  << _playerList.at(2).getScore() << std::setfill(' ');

    else std::cout << std::setw(39) << " ";

    std::cout << "   | |\n";
    std::cout << "   | |  ";

    if (_nPlayers >= 4)
        std::cout << std::setw(37) << std::left << _playerList.at(3).getName() << std::setfill('0') << std::right
                  << std::setw(2)
                  << _playerList.at(3).getScore() << std::setfill(' ');

    else std::cout << std::setw(39) << " ";

    std::cout << "   | |\n";
    std::cout << "   | |                                            | |\n"
                 "   | |                                            | |\n"
                 "   | |                                            | |\n"
                 " __| |____________________________________________| |__\n"
                 "(__   ____________________________________________   __)\n"
                 "   | |                                            | |\n\n";


    std::cout << "Thanks for playing this game. Press ENTER to go back to the main menu\n";
    std::cin.ignore();

}

/**
 * @brief shows the board on the intended position
 */
void Game::showBoard() {

    goToXY(0, 0);
    _board.showBoard(std::cout);

}

/**
 * @brief shows info about the players who are not currently playing on the side
 */
void Game::showOthersHands() {


    std::vector<Player> notPlaying{};
    for (unsigned long i = 0; i < _nPlayers; i++) {
        if (i != _currentN) notPlaying.push_back(_playerList.at(i));
    }
    while (notPlaying.size() != 3) {
        notPlaying.emplace_back();
    }

    goToXY(WIDTH, 2);
    std::cout << notPlaying.at(0).getName();
    goToXY(WIDTH, 3);
    if (_nPlayers >= 2) std::cout << "Score: " << notPlaying.at(0).getScore();
    goToXY(WIDTH, 4);
    notPlaying.at(0).showHand();

    goToXY(WIDTH, 6);
    std::cout << notPlaying.at(1).getName();
    goToXY(WIDTH, 7);
    if (_nPlayers >= 3) std::cout << "Score: " << notPlaying.at(1).getScore();
    goToXY(WIDTH, 8);
    notPlaying.at(1).showHand();

    goToXY(WIDTH, 10);
    std::cout << notPlaying.at(2).getName();
    goToXY(WIDTH, 11);
    if (_nPlayers == 4) std::cout << "Score: " << notPlaying.at(2).getScore();
    goToXY(WIDTH, 12);
    notPlaying.at(2).showHand();

}

/**
 * @brief shows all hands
 */
void Game::showAllHands() {

    showOthersHands();

    goToXY(2, HEIGHT + 3);
    _currentP.showHand();
    goToXY(2, HEIGHT + 4);
    std::cout << "You (" << _currentP.getScore() << " points)";

}

/**
 * @brief get the width needed for padding
 * @return the width of the board
 */
size_t Game::getSize() {
    return _board.getnLines();
}


/**
 * @brief shows a Player's error (bad move) in the reserved place for that
 */
void Game::showPlayerError() {

    goToXY(WIDTH, 15);
    clearLineAndGoUp();  // in case there is any other message there

    printMessage(_currentP.error, 1);
    _currentP.error = "";  // the error was shown, so it's state is reset

    showBoard();  // clearing the line may clear part of the board

}



/**
 * @brief gives a hint
 */
void Game::giveHint() {

    goToXY(WIDTH, 15);
    clearLineAndGoUp();  // in case there is any other message there

    printMessage(_currentP.getHint(_board), 3);

    showBoard();  // clearing the line may clear part of the board

}
