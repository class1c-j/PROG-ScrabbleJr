//
// Created by ritam on 04/05/20.
//

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
