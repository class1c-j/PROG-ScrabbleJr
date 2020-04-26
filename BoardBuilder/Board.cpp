//
// Created by ritam on 25/04/20.
//

#include "Board.h"
#include <iostream>
#include <iomanip>

Board::Board(unsigned rows, unsigned cols) {
    numLines = rows;
    numCols = cols;

    // resize the board vector according to the specified dimensions
    board.resize(numLines);
    for (auto & i : board) {
        i.resize(numCols);
    }

    // initialize it with '0' in every position to mark that there are no pieces placed
    for (size_t i = 0; i < numLines; i ++) {
        for (size_t j = 0; j < numCols; j++) {
            board.at(i).at(j) = '0';
        }
    }

}

void Board::showBoard() {

    const int W = 2;  // edit to change how close together the letters are in the board

    // lowercase letters
    std::cout << "\n    ";
    for (int i = 0; i < numCols; i ++) {
        std::cout << std::left << std::setw(W) << char(i + 97) << " ";
    }

    std::cout << '\n';
    std::cout << std::setfill('_') << std::setw(numCols * W + numCols + W) << std::left << "    " << "\n";

    for (int i = 0; i < numLines; i ++) {

        std::cout << std::setfill(' ') << std::right << std::setw(2) << char(i + 65) << " |";   // uppercase letters

        for (int j = 0; j < numCols; j ++) {
            if (board.at(i).at(j) == '0') {  // do not show the 0 in empty positions
                std::cout << std::left << std::setw(W + 1) << " ";
            } else {
                std::cout << std::left << std::setw(W) << board.at(i).at(j) << ' ';
            }
        }

        std::cout << '\n';
    }

}

// coords -> {linha, coluna} -> {maiuscula, minuscula}
void Board::insertWord(const std::string& word, std::pair<char, char> coords, const char& orientation) {
    bool available = verifyBoard(word, coords, orientation);
    if (available) {
        if (orientation == 'V') {  // a coluna é sempre a mesma, apenas iterar a linha
            for (int i = coords.first; i < coords.first + word.size(); i++) {
                board.at(i).at(coords.second) = word.at(i - coords.first);
            }
        }
        else if (orientation == 'H') {  // linha é sempre a mesma, apenas iterar a coluna
            for (int i = coords.second; i < coords.second + word.size(); i++) {
                board.at(coords.first).at(i) = word.at(i - coords.second);
            }
        }
    }
}

std::vector<char> Board::getLine(unsigned int line) {
    return board.at(line);
}

std::vector<char> Board::getCol(unsigned int col) {
    std::vector<char> column;
    for (auto line : board) {
        column.push_back(line.at(col));
    }
    return column;
}

bool Board::verifyBoard(const std::string& word, std::pair<char, char> coords, const char& orientation) {
    if (orientation == 'H') {
        if ((numCols - coords.second) < word.size()) {
            std::cout << "The inserted word does not respect the dimensions of the board!" << std::endl;
            return false;
        }
        else {
            std::vector<char> line = getLine(coords.first);
            for (int i = coords.second; i <= word.size(); i++) {
                if (line.at(i) != '0') {
                    if (word.at(i - coords.second) != line.at(i)) {
                        std::cout << "The inserted word does not respect the words alrealdy inserted in the board!" << std::endl;
                        return false;
                    }
                }
            }
        }
    }
    else {
        if ((numLines - coords.first) < word.size()) {
            std::cout << "The inserted word does not respect the dimensions of the board!" << std::endl;
            return false;
        }
        else {
            std::vector<char> col = getCol(coords.second);
            for (int i = coords.first; i <= word.size(); i++) {
                if (col.at(i) != '0') {
                    if (word.at(i - coords.first) != col.at(i)) {
                        std::cout << "The inserted word does not respect the words alrealdy inserted in the board!" << std::endl;
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
