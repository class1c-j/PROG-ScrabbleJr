#define NOMINMAX
#include "gameIO.h"

/**
 * @brief reads a letter to play or exchange but also detects if the player asked for a hint
 * @param c - the input
 */
void readLetter(std::string &c) {

    std::transform(c.begin(), c.end(), c.begin(), ::tolower);

    std::cout << "Choose letter ? ";
    std::cin >> c;
    while (std::cin.fail() || (c.size() > 1 && c != "hint")) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\033[A\033[2K";
        std::cout << "Invalid line choice. Try again: ";
        std::cin >> c;
    }

    std::cout << "\033[A\033[2K";
}



/**
 * @brief reads the coordinates where the player wants to play
 * @param coords - the coordinates
 * @param board  - the board (used for placing the messages in the right place and checking if the coords are valid)
 */
void readCoordinates(std::pair<char, char> &coords, const Board &board) {

    gotoxy(board.getnCols() + 30, board.getnLines() + 5);
    std::cout << "Line ? ";
    std::cin >> coords.first;

    while (std::cin.fail() || !isalpha(coords.first) || coords.first - 65 >= board.getnLines() ||
           std::cin.peek() != '\n') {
        if (std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "User chose to close input.\n";
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\033[A\033[2K";
            gotoxy(board.getnCols() + 30, board.getnLines() + 5);
            std::cout << "Invalid line choice. Try again: ";
            std::cin >> coords.first;
        }
    }
    std::cout << "\033[A\033[2K";

    gotoxy(board.getnCols() + 30, board.getnLines() + 5);
    std::cout << "Column ? ";
    std::cin >> coords.second;
    while (std::cin.fail() || !isalpha(coords.second) || coords.second - 97 >= board.getnCols() ||
           std::cin.peek() != '\n') {
        if (std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "User chose to close input.\n";
            break;
        } else {
            std::cout << "\033[A\033[2K";
            std::cin.clear();
            gotoxy(board.getnCols() + 30, board.getnLines() + 5);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid column choice. Try again: ";
            std::cin >> coords.second;
        }

    }

    coords.first -= 65;
    coords.second -= 97;
}



/**
 * @brief reads the number of players
 * @param number - the number of players
 */
void readNumberPlayers(size_t &number) {
    std::cout << "Number of players ? ";
    std::cin >> number;

    while (std::cin.fail() || number < 0 || number > 4 || (std::cin.peek() != '\n' && !isdigit(std::cin.peek()))) {
        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "User chose to close the input.\n";
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Failed to read number. Try again: ";
                std::cin >> number;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Must choose between 1 and 4 players. Try again: ";
            std::cin >> number;
        }
    }
}


/**
 * @brief reads the names of the players
 * @param number - the number of players to be read
 * @return a vector with all the names
 */
std::vector<std::string> readPlayersNames(size_t number) {

    std::cin.ignore();  // getline will be used, so we have to remove the \n that could be in the buffer

    std::vector<std::string> players(number);

    for (size_t i = 0; i < number; ++i) {
        std::string temp;
        std::cout << "Player " << i << ", name ? ";
        std::getline(std::cin, temp);
        players.at(i) = temp;
    }

    return players;
}
