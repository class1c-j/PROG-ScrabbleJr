#define NOMINMAX
#include "gameIO.h"

/**
 * @brief reads a letter to play or exchange but also detects if the player asked for a hint
 * @param c - the input
 * @param horizontalPadding - the column where the text should be shown
 * @param verticalPadding - the line where the text should be shown
 */
void readLetter(std::string& c, size_t horizontalPadding, size_t verticalPadding) {

    std::transform(c.begin(), c.end(), c.begin(), ::tolower);
    
    std::cout << "Choose letter ? ";
    std::cin >> c;

    while (std::cin.fail() || (c.size() > 1 && c != "hint")) {

        goToXY(horizontalPadding, verticalPadding);
        if (std::cin.eof()) {
            std::cin.clear();
            clearLineAndGoUp();
            std::cout << "User chose to close input. Please enter the letter again : ";
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearLineAndGoUp();
            std::cout << "Invalid tile. Try again: ";
        }
        std::cin >> c;
    }
    clearLineAndGoUp();
}




/**
 * @brief reads the coordinates where the player wants to play
 * @param coords - the coordinates
 * @param board  - the board (used for placing the messages in the right place and checking if the coords are valid)
 */
void readCoordinates(std::pair<char, char>& coords, const Board& board) {

    goToXY(board.getNumberCols() + 30, board.getNumberLines() + 5);
    std::cout << "Line ? ";
    std::cin >> coords.first;

    while (std::cin.fail() || !isalpha(coords.first) || static_cast<size_t>(coords.first - 'A') >= board.getNumberLines() ||
        std::cin.peek() != '\n') {

        if (std::cin.eof()) {
            std::cin.clear();
            clearLineAndGoUp();
            goToXY(board.getNumberCols() + 30, board.getNumberLines() + 5);
            std::cout << "User chose to close input. Please enter the line again: ";
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearLineAndGoUp();
            goToXY(board.getNumberCols() + 30, board.getNumberLines() + 5);
            std::cout << "Invalid line choice. Try again: ";
        }
        std::cin >> coords.first;
    }

    clearLineAndGoUp();

    goToXY(board.getNumberCols() + 30, board.getNumberLines() + 5);
    std::cout << "Column ? ";
    std::cin >> coords.second;
    while (std::cin.fail() || !isalpha(coords.second) || static_cast<size_t>(coords.second - 'a') >= board.getNumberCols() ||
        std::cin.peek() != '\n') {
        if (std::cin.eof()) {
            std::cin.clear();
            clearLineAndGoUp();
            goToXY(board.getNumberCols() + 30, board.getNumberLines() + 5);
            std::cout << "User chose to close input. Please enter the column again: ";
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearLineAndGoUp();
            goToXY(board.getNumberCols() + 30, board.getNumberLines() + 5);
            std::cout << "Invalid column choice. Try again: ";
        }
        std::cin >> coords.second;

    }

    coords.first -= 65;
    coords.second -= 97;
}


/**
 * @brief reads the number of players
 * @param number - the number of players
 */
void readNumberPlayers(size_t& number) {
    std::cout << "Number of players ? ";
    std::cin >> number;

    while (std::cin.fail() || number > 4 || (std::cin.peek() != '\n' && !isdigit(std::cin.peek()))) {
        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cout << "User chose to close the input.\n";
                std::cout << "Please enter the number of players again: ";
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Failed to read number. Try again: ";
            }
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Must choose between 1 and 4 players. Try again: ";
        }
        std::cin >> number;
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
