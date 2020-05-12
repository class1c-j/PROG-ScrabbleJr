#define NOMINMAX
#include "gameIO.h"

void readLetter(char &c) {

    std::cout << "Choose letter ? ";
    std::cin >> c;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\033[A\033[2K";
        std::cin >> c;
    }

    std::cout << "\033[A\033[2K";
}

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

            std::cout << "\033[A\033[2K";
            gotoxy(2, board.getnLines() + 5);
            std::cout << "Your tiles: ";  // clearing the line causes this to disappear
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            gotoxy(board.getnCols() + 30, board.getnLines() + 5);
            std::cout << "Invalid line choice. Try again: ";
            std::cin >> coords.first;
        }
    }
    std::cout << "\033[A\033[2K";

    gotoxy(2, board.getnLines() + 5);
    std::cout << "Your tiles: ";  // clearing the line causes this to disappear


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
            gotoxy(board.getnCols() + 30, board.getnLines() + 6);
            std::cout << "\033[A\033[2K";
            gotoxy(2, board.getnLines() + 5);
            std::cout << "Your tiles: ";  // clearing the line causes this to disappear
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

std::vector<std::string> readPlayersNames(unsigned number) {

    std::vector<std::string> players(number);

    for (size_t i = 0; i < number; ++i) {
        std::string temp;
        std::cout << "Player " << i << ", name ? ";
        std::getline(std::cin, temp);
        players.at(i) = temp;
    }

    return players;
}
