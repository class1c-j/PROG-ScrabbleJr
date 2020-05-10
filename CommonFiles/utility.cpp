
#include "utility.h"

void setBackgroundColor(unsigned colour) {
#ifdef _WIN32
    // YELLOY -> 14, // READ -> 4, // WHITE -> 15
    if (colour == 1) {
        HANDLE hcon= GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon, BACKGROUND_GREEN);
    } else {
        HANDLE hcon= GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hcon, 15);
    }
#else
    std::vector<std::string> colours = {"\033[0m", "\033[0;42m"};
    std::cout << colours.at(colour);
#endif
}

bool operator<(const Player &player1, const Player &player2) {
    return player1.getScore() < player2.getScore();
}
