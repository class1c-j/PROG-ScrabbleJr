
#include "utility.h"

// TODO:  windows compatibility

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

void clearScreen() {
#ifdef _WIN32
    COORD coordScreen = { 0, 0 };  // upper left corner
    DWORD cCharsWritten;
    DWORD dwConSize;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hCon, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    // fill with spaces
    FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hCon, &csbi);
    FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    // cursor to upper left corner
    SetConsoleCursorPosition(hCon, coordScreen);
#else
    std::cout << "\033[2J\033[1;1H";
#endif
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
#ifdef _WIN32
    // YELLOY -> 14, // READ -> 4, // WHITE -> 15
    std::vector<int> colours = {15, 14, 4};
    HANDLE hcon= GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon, colours.at(colour));

    std::cout << message;

    SetConsoleTextAttribute(hcon, colours.at(0));
#else
    std::vector<std::string> colours = {"\033[0m", "\033[1;33m", "\033[0;31m"};
    std::cout << colours.at(colour);
    std::cout << message;
    std::cout << colours.at(0);
#endif
}

void gotoxy(int x, int y) {
    std::ostringstream oss;
    oss << "\033["<< y << ";"<< x << "H";
    std::cout << oss.str();
}