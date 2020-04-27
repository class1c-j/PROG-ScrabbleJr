#include <iostream>
#include <fstream>
#include <vector>
#include "Board.h"
#include "Menu.h"
#include "editIO.h"

void showName() {
    std::cout << "  ___                         _     ___          _   _      _             \n"
                 " | _ )  ___   __ _   _ _   __| |   | _ )  _  _  (_) | |  __| |  ___   _ _ \n"
                 " | _ \\ / _ \\ / _` | | '_| / _` |   | _ \\ | || | | | | | / _` | / -_) | '_|\n"
                 " |___/ \\___/ \\__,_| |_|   \\__,_|   |___/  \\_,_| |_| |_| \\__,_| \\___| |_|  \n"
                 "                                                                          \n";
}

void createBoard(Board &board) {

    unsigned numLines, numCols;
    readDimentions(numLines, numCols);
    board = Board(numLines, numCols);

    board.showBoard();

    char l{}, c{}, orientation{};
    auto coords = std::make_pair(l, c);

    std::string word{};

    const Menu createMenu{"This is edit mode. Choose from the options below to create your board.\n",
                          "Invalid choice! ",
                          {{"Insert", [&board, &coords, &orientation, &word] {
                              readWord(word);
                              readCoordinates(coords, board);
                              readOrientation(orientation);
                              board.insertWord(word, coords, orientation);
                          }}}
    };
    createMenu();

    board.showBoard();
}

int main() {
    Board board;
    showName();
    const Menu mainMenu{"Welcome to the Board Builder Programme. Here you can build boards for ScrabbleJr.\n",
                        "Invalid choice! ",
                        {{"Create", [&board] { createBoard(board); }}}
    };
    mainMenu();

}