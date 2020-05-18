#define NOMINMAX
#include "utility.h"


void clearScreen() {
    std::cout << "\033[2J\033[1;1H";

}

void clearLineAndGoUp() {
    std::cout << "\033[A\033[2K";
}


void searchFile(std::string &fileName) {

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

void printMessage(const std::string& message, unsigned colour) {

    std::vector<std::string> colours = {"\033[0m", "\033[1;33m", "\033[0;31m", "\033[0;92m"};
    std::cout << colours.at(colour);
    std::cout << message;
    std::cout << colours.at(0);

}
/**
 * @brief moves the cursor to a given position (x, y)
 * @param x
 * @param y
 */
void goToXY(size_t x, size_t y) {
    std::ostringstream oss;
    oss << "\033["<< y << ";"<< x << "H";
    std::cout << oss.str();
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
