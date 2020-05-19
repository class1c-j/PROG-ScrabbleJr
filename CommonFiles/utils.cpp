#define NOMINMAX
#define _YELLOW "\033[1;33m"
#define _WHITE "\033[0m"
#define _GREEN "\033[0;92m"
#define _RED "\033[0;31m"
#define _ENGINEERING "\033[38;2;140;45;25m"
#include "utils.h"


/**
 * @brief uses an ANSI escape sequence to clear the screen
 */
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";

}

/**
 * @brief uses an ANSI escape sequence to clear the current line and go up
 */
void clearLineAndGoUp() {
    std::cout << "\033[A\033[2K";
}


/**
 * @brief searches the proper location for the board to open
 * @param fileName the name of the board
 */
void searchBoard(std::string &fileName) {

    std::cout << "We included a sample board named 'test' for testing purposes.\n";
    std::cout << "If, however, you want to choose an imported board, make sure it is in the UserBoards directory\n\n";
    std::cout << "Search for board (without file extension) : ";
    std::cin >> fileName;
    fileName = "../UserBoards/" + fileName + ".txt";


    while (true) {
        std::ifstream file(fileName);
        if (!file.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Board not found. Try again : ";
            std::cin >> fileName;
            fileName = "../UserBoards/" + fileName + ".txt";
        } else {
            break;
        }
    }
}

/**
 * @brief shows a colored message
 * @param message the text
 * @param colour the colour
 */
void printMessage(const std::string& message, unsigned colour) {

    // white, yellow, red, green
    std::vector<std::string> colours = {_WHITE, _YELLOW, _RED, _GREEN, _ENGINEERING};
    std::cout << colours.at(colour);
    std::cout << message;
    std::cout << colours.at(0);

}
/**
 * @brief moves the cursor to a given position (x, y)
 * @param x horizontal
 * @param y vertical
 */
void goToXY(size_t x, size_t y) {
    std::ostringstream oss;
    oss << "\033["<< y << ";"<< x << "H";
    std::cout << oss.str();
}

void clearFromCursor() {
    std::cout << "\033[0K";
}

#ifdef _WIN32

int windowsSetup() {
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return GetLastError();
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return GetLastError();
    }

    return 0;

}

#endif
