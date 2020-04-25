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
    board.at(4).at(6) = 'A';
    board.at(3).at(6) = 'A';
    board.at(4).at(7) = 'A';
}

void Board::showBoard() {

    const int W = 2;  // edit to change how close together the letters are in the board

    // lowercase letters
    std::cout << "\n    ";
    for (int i = 0; i < numCols; i ++) {
        std::cout << std::left << std::setw(W) << char(i + 97) << " ";
    }

    std::cout << '\n';
    std::cout << std::setfill('_') << std::setw(numCols * W + numCols + W) << std::left << "   " << "_\n";

    for (int i = 0; i < numLines; i ++) {

        std::cout << std::setfill(' ') << std::left << std::setw(W) << char(i + 65) << "| ";   // uppercase letters

        for (int j = 0; j < numCols; j ++) {
            if (board.at(i).at(j) == '0') {  // do not show the 0 in empty positions
                std::cout << std::left << std::setw(W) << ' ';
            } else {
                std::cout << std::left << std::setw(W) << board.at(i).at(j) << ' ';
            }
        }

        std::cout << '\n';
    }

}