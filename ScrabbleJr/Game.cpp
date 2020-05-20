#define NOMINMAX
#include "Game.h"

Game::Game() = default;

Game::Game(const std::vector<Player> &players, const Board &board, Pool pool)
        : m_playerList(players), m_board(board), m_pool(std::move(pool)), m_currentN(0),
          m_currentP(m_playerList.at(m_currentN)), m_nPlayers(players.size()), m_width(board.getNumberCols() + 30),
          m_height(board.getNumberLines() + 3) {

}

//======================================================================================================================
/**
 * @brief get the width needed for padding
 * @return the width of the board
 */
size_t Game::getSize() {
    return m_board.getNumberLines();
}

//======================================================================================================================
/**
 * @brief check is the game is finished
 * @return true if the game has ended, false otherwise
 */
bool Game::isFinished() {
    return m_board.isFinished();
}

//======================================================================================================================
/**
 * @brief calculates and sets the next player to play
 */
void Game::nextPlayer() {

    m_currentN = (m_currentN + 1) % m_nPlayers;
    m_currentP = m_playerList.at(m_currentN);

    // in case one player goes out of tiles, his turn is skipped
    while (m_currentP.getHand().empty() && !isFinished()) {

        goToXY(m_width, m_height);
        clearLineAndGoUp();
        printMessage(m_currentP.getName() + " has no tiles. Press ENTER to pass ...", 0);
        showBoard();

        goToXY(m_width, 16);
        std::cin.ignore();

        m_currentN = (m_currentN + 1) % m_nPlayers;
        m_currentP = m_playerList.at(m_currentN);

    }

}

//======================================================================================================================
/**
 * @brief gets a vector with all the winners
 * @return all of the winners of a scrabble game
 */
std::vector<Player> Game::getWinners() {

    size_t bestScore = m_playerList.at(0).getScore();

    std::vector<Player> winners{};

    for (const auto &player : m_playerList) {
        if (player.getScore() == bestScore) winners.push_back(player);
    }

    return winners;

}

//======================================================================================================================
/**
 * @brief makes a play taking into consideration how many tiles can be played and updates the player state
 */
void Game::makePlay() {

    size_t playable = m_currentP.getPlayable(m_board).size();  // count with the possible plays

    if (playable == 0) {

        goToXY(m_width, m_height);
        clearLineAndGoUp();

        if (!m_pool.isEmpty()) {
            if (m_pool.getSize() == 1) {  // if there is only one tile in the pool, exchange it

                if (!m_currentP.isBot()) printMessage(m_currentP.getName() + ", exchange 1 tile", 0);
                else showBotMessage(0);  // bot will exchange 1 tile
                exchangeTile();

            } else {  // exchange 2 tiles

                if(m_currentP.isBot()) showBotMessage(1);  // bot will exchange 2 tiles
                for (int i = 0; i < 2; i++) {

                    std::stringstream msgStream{};
                    msgStream << m_currentP.getName() << ", exchange " << 2 - i << " tiles";

                    if (!m_currentP.isBot()) printMessage(msgStream.str(), 0);
                    exchangeTile();
                }
            }

        }
    }

    // after the exchange, the player might be able to play, so the count is updated
    playable = m_currentP.getPlayable(m_board).size();

    if (playable == 1) {

        goToXY(m_width, m_height);
        clearLineAndGoUp();

        if (!m_currentP.isBot()) printMessage("It's your turn, " + m_currentP.getName() + ", play 1 tile.", 0);
        else showBotMessage(2);  // bot will play 1 tile

        // place 1 tile, take 1 tile
        playTile();
        m_pool.dealHand(1, m_currentP);

    } else if (playable >= 2) {

        if(m_currentP.isBot()) showBotMessage(3);  // bot will play 2 tiles

        // place 2 tiles, take 2 tiles
        for (int i = 0; i < 2; i++) {

            std::stringstream msgStream{};
            msgStream << m_currentP.getName() << ", play " << 2 - i << " tiles";
            goToXY(m_width, m_height);
            clearLineAndGoUp();
            if (!m_currentP.isBot()) printMessage(msgStream.str(), 0);
            playTile();

        }

        m_pool.dealHand(2, m_currentP);

    } else {

        goToXY(m_width, m_height);
        clearLineAndGoUp();

        // There are no possible plays, the player must pass
        if (!m_currentP.isBot()) printMessage("It's your turn, " + m_currentP.getName() + ", press ENTER to pass.", 0);
        else showBotMessage(4);
        showBoard();
        std::cin.ignore();

    }

    // update player state
    m_playerList.at(m_currentN) = m_currentP;

}

//======================================================================================================================
/**
 * @brief exchanges a player's tile for a new one in case he has no possible plays and the pool is not empty
 * and updates the player state
 */
void Game::exchangeTile() {

    std::string input{};
    char tile{};

    showBoard();
    showAllPlayersInfo();

    if (m_currentP.isBot()) tile = m_currentP.getHand().front();

    while (!m_currentP.hasTile(tile) || input.size() > 1) {

        showBoard();
        showAllPlayersInfo();

        goToXY(m_width, m_height + 3);
        clearLineAndGoUp();

        readLetter(input, m_width, m_height + 3);

        while (input == "hint") {
            readLetter(input, m_width, m_height + 3); // hints are not allowed in this phase
        }

        tile = input.front();

        if (!m_currentP.hasTile(tile)) {

            showPlayerError();

        } else break;

    }

    m_currentP.removeTile(tile);  // take the tile from the player
    m_pool.addTile(tile);        // add the tile to the pool

    m_pool.dealHand(1, m_currentP);  // give a new tile to the player

    clearScreen();
    showBoard();
    showAllPlayersInfo();

    // update player state
    m_playerList.at(m_currentN) = m_currentP;

}

//======================================================================================================================
/**
 * @brief plays a tile
 */
void Game::playTile() {

    std::string input{};
    char tile{};
    std::pair<char, char> coords{};

    if (m_currentP.isBot()) {

        coords = m_currentP.getPlayable(m_board).front();
        tile = m_board.getLetter(coords);
    }

    while (!m_currentP.isValidMove(tile, coords, m_board)) {

        showBoard();
        showAllPlayersInfo();

        goToXY(m_width, m_height + 3);
        clearLineAndGoUp();

        readLetter(input, m_width, m_height + 3);

        while (input == "hint") {

            showHint();

            goToXY(m_width, m_height + 3);
            clearLineAndGoUp();

            readLetter(input, m_width, m_height + 3);

        }

        readCoordinates(coords, m_board);

        tile = input.at(0);

        if (!m_currentP.isValidMove(tile, coords, m_board)) {

            showPlayerError();

        } else {
            break;
        }

    }


    m_currentP.play(tile, coords, m_board);

    // check if the played scored
    for (int i = 0; i < m_board.getNumberOfCompletedWordsByPlay(coords); i++) {
        m_currentP.incrementScore();
    }

    clearScreen();
    showBoard();
    showAllPlayersInfo();

    // update player state
    m_playerList.at(m_currentN) = m_currentP;

}

//======================================================================================================================
/**
 * @brief shows the board on the intended position
 */
void Game::showBoard() {

    goToXY(0, 0);
    m_board.show(std::cout);

}

//======================================================================================================================
/**
 * @brief shows all hands, scores and names
 */
void Game::showAllPlayersInfo() {

    std::vector<Player> notPlaying{};
    for (unsigned long i = 0; i < m_nPlayers; i++) {
        if (i != m_currentN) notPlaying.push_back(m_playerList.at(i));
    }
    while (notPlaying.size() != 3) {
        notPlaying.emplace_back();
    }

    goToXY(m_width, 2);
    std::cout << notPlaying.at(0).getName();
    goToXY(m_width, 3);
    if (m_nPlayers >= 2) std::cout << "Score: " << notPlaying.at(0).getScore();
    goToXY(m_width, 4);
    notPlaying.at(0).showHand();

    goToXY(m_width, 6);
    std::cout << notPlaying.at(1).getName();
    goToXY(m_width, 7);
    if (m_nPlayers >= 3) std::cout << "Score: " << notPlaying.at(1).getScore();
    goToXY(m_width, 8);
    notPlaying.at(1).showHand();

    goToXY(m_width, 10);
    std::cout << notPlaying.at(2).getName();
    goToXY(m_width, 11);
    if (m_nPlayers == 4) std::cout << "Score: " << notPlaying.at(2).getScore();
    goToXY(m_width, 12);
    notPlaying.at(2).showHand();

    goToXY(2, m_height + 3);
    m_currentP.showHand();
    goToXY(2, m_height + 4);
    std::cout << m_currentP.getName() << " (" << m_currentP.getScore() << " points)";

}

//======================================================================================================================
/**
 * @brief shows a Player's error (bad move) in the reserved place for that
 */
void Game::showPlayerError() {

    goToXY(m_width, 15);
    clearLineAndGoUp();  // in case there is any other message there

    printMessage(m_currentP.error, 1);
    m_currentP.error = "";  // the error was shown, so it's state is reset

    showBoard();  // clearing the line may clear part of the board

}

//======================================================================================================================

void Game::showBotMessage(size_t messageCode) {

    std::string message = "BOT " + m_currentP.getName() + m_botMessages.at(messageCode);

    goToXY(m_width, m_height);
    clearLineAndGoUp();
    goToXY(m_width, m_height);
    printMessage(message, 0);
    showBoard();
    goToXY(m_width, m_height + 1);

    while (std::cin.rdbuf()->in_avail()) {
        std::string fromBuffer{};  // sometimes the last user play will leave things in the buffer, so to make
        std::getline(std::cin, fromBuffer);  // sure that the ignore will work, it is emptied
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//======================================================================================================================
/**
 * @brief gives a hint
 */
void Game::showHint() {

    goToXY(m_width, 15);
    clearLineAndGoUp();  // in case there is any other message there
    printMessage(m_currentP.getHint(m_board), 3);
    showBoard();  // clearing the line may clear part of the board

}

//======================================================================================================================
/**
 * @brief show a leaderboard and som ascii art as an end screen after the board is finished
 */
void Game::showLeaderboard() {

    clearScreen();
    std::sort(m_playerList.begin(), m_playerList.end(), std::greater<>());

    std::cout << " __| |____________________________________________| |__\n"
                 "(__   ____________________________________________   __)\n"
                 "   | |                                            | |\n"
                 "   | |               BOARD FINISHED               | |\n"
                 "   | |                                            | |\n";
    std::cout << "   | |  ";

    if (m_nPlayers >= 1)
        std::cout << std::setw(37) << std::left << m_playerList.at(0).getName() << std::setfill('0') << std::right
                  << std::setw(2)
                  << m_playerList.at(0).getScore() << std::setfill(' ');

    else std::cout << std::setw(39) << " ";

    std::cout << "   | |\n";
    std::cout << "   | |  ";

    if (m_nPlayers >= 2)
        std::cout << std::setw(37) << std::left << m_playerList.at(1).getName() << std::setfill('0') << std::right
                  << std::setw(2)
                  << m_playerList.at(1).getScore() << std::setfill(' ');

    else std::cout << std::setw(39) << " ";

    std::cout << "   | |\n";
    std::cout << "   | |  ";

    if (m_nPlayers >= 3)
        std::cout << std::setw(37) << std::left << m_playerList.at(2).getName() << std::setfill('0') << std::right
                  << std::setw(2)
                  << m_playerList.at(2).getScore() << std::setfill(' ');

    else std::cout << std::setw(39) << " ";

    std::cout << "   | |\n";
    std::cout << "   | |  ";

    if (m_nPlayers >= 4)
        std::cout << std::setw(37) << std::left << m_playerList.at(3).getName() << std::setfill('0') << std::right
                  << std::setw(2)
                  << m_playerList.at(3).getScore() << std::setfill(' ');

    else std::cout << std::setw(39) << " ";

    std::cout << "   | |\n";
    std::cout << "   | |                                            | |\n"
                 "   | |                                            | |\n"
                 "   | |                                            | |\n"
                 " __| |____________________________________________| |__\n"
                 "(__   ____________________________________________   __)\n"
                 "   | |                                            | |\n\n";

    if (getWinners().size() == 1) std::cout << getWinners().at(0).getName() << " won. Congrats!\n";

    else {

        std::cout << "This game ended in a tie. Congrats to: | ";
        for (const auto &winner : getWinners()) {
            std::cout << winner.getName() << " | ";
        }
        std::cout << "!\n";
    }

    std::cout << "Thanks for playing this game. Press ENTER to go back to the main menu\n";

    while (std::cin.rdbuf()->in_avail()) {
        std::string fromBuffer{};  // sometimes the last user play will leave things in the buffer, so to make
        std::getline(std::cin, fromBuffer);  // sure that the ignore will work, it is emptied
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

}

//======================================================================================================================