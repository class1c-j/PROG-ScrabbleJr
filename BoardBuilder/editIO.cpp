
#include "editIO.h"
#include <limits>
#include <iostream>
#define NOMINMAX

#ifdef _WIN32  // Windows compatibility
#include <windows.h>
#include <cstdlib>
#endif

void readDimensions(unsigned &lines, unsigned &cols) {
    std::cout << "Number of lines ? ";
    std::cin >> lines;
    while (std::cin.fail() || lines < 0 || lines > 20 || (std::cin.peek() != '\n' && !isdigit(std::cin.peek()))) {
        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "User chose to close the input.\n";
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Failed to read number. Try again: ";
                std::cin >> lines;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "That is not a valid number of lines. Remember, maximum allowed is 20. Try again: ";
            std::cin >> lines;
        }
    }

    std::cout << "Number of columns ? ";
    std::cin >> cols;
    while (std::cin.fail() || cols < 0 || cols > 20 || (std::cin.peek() != '\n' && !isdigit(std::cin.peek()))) {
        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "User chose to close the input.\n";
                break;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Failed to read number. Try again: ";
                std::cin >> cols;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "That is not a valid number of columns. Remember, maximum allowed is 20. Try again: ";
            std::cin >> cols;
        }
    }
}


void readCoordinates(std::pair<char, char> &coords, const Board &board) {
    std::cout << "Starting position line ? ";
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

    std::cout << "Starting position column ? ";
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

void readOrientation(char &orientation) {
    std::cout << "Orientation ? ";
    std::cin >> orientation;
    while (std::cin.fail() || (orientation != 'V' && orientation != 'H') || std::cin.peek() != '\n') {
        if (std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "User chose to close input.\n";
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid orientation. Insert V for vertical or H for horizontal. Try again: ";
            std::cin >> orientation;
        }
    }
}

void readWord(std::string &word) {
    std::cout << "Word ? ";
    std::cin >> word;

    while (true) {
        while (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "User chose to close input.\n";
                break;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Remember, your word must not have any spaces. Try again: ";
                std::cin >> word;
            }
        }
        for (const char i : word) {
            if (!isalpha(i)) {
                std::cout << "Word must not include numbers or symbols. Try again: ";
                std::cin >> word;
            }
        }
        break;
    }

    for (int i : word) {
        i = tolower(i);
    }

}

void readFileName(std::string &fileName) {
    std::cout
            << "Be aware of your operating system's naming limitations.\nWindows - no reserved names, no <>:\\/|?*,"
               " don't end with \' \' or \'.\'\nLinux - no \'/\' \n";

    std::cout << "File name (without extension) ? ";
    std::cin >> fileName;
    fileName += ".txt";


    while (true) {
        std::ofstream test(fileName);
        if (std::cin.fail() || !test) {
            std::cout << "Invalid file name. Try again: ";
            std::cin >> fileName;
            fileName += ".txt";
        } else {
            break;
        }
    }
}

