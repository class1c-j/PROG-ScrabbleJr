
#include "gameIO.h"

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

void readLetter(char &c) {
    std::cout << "Choose letter ? ";
    std::cin >> c;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> c;
    }
}

void readCoordinates(std::pair<char, char> &coords, const Board &board) {
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
            std::cout << "Invalid line choice. Try again: ";
            std::cin >> coords.first;
        }
    }

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
            std::cout << "Invalid column choice. Try again: ";
            std::cin >> coords.second;
        }

    }

    coords.first -= 65;
    coords.second -= 97;
}