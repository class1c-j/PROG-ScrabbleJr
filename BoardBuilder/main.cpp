#include <iostream>
#include <fstream>
#include <vector>
#include "Board.h"
#include "Menu.h"
#include "editIO.h"


void showMainMenu(Board board);
void saveBoard(std::string &filename, Board board);
void createBoard(Board &board);
void editBoard(Board &board);

int main() {
    Board board = Board();
    showMainMenu(board);
}



void showName() {
    std::cout << "  ___                         _     ___          _   _      _             \n"
                 " | _ )  ___   __ _   _ _   __| |   | _ )  _  _  (_) | |  __| |  ___   _ _ \n"
                 " | _ \\ / _ \\ / _` | | '_| / _` |   | _ \\ | || | | | | | / _` | / -_) | '_|\n"
                 " |___/ \\___/ \\__,_| |_|   \\__,_|   |___/  \\_,_| |_| |_| \\__,_| \\___| |_|  \n"
                 "                                                                          \n";
}

void showMainMenu(Board board) {

    clearScreen();

    showName();
    const Menu mainMenu{"Welcome to the Board Builder Programme. Here you can build boards for ScrabbleJr.\n",
                        "Invalid choice! ",
                        {{"Create a new board", [&board] { createBoard(board); }},
                         {"Edit existing board", [&board] { editBoard(board); },}}
    };
    mainMenu();

}

void showEditMenu(Board &board, bool &running) {

    char l{}, c{}, orientation{};
    auto coords = std::make_pair(l, c);

    std::string word{}, name{};


    const Menu editMenu{"This is edit mode. Choose from the options below to create your board.\n",
                          "Invalid choice! ",
                          {{"Insert", [&board, &coords, &orientation, &word] {
                              readWord(word);
                              readCoordinates(coords, board);
                              readOrientation(orientation);
                              if (board.verifyWord(word, coords, orientation)) {
                                  board.insertWord(word, coords, orientation);
                              }
                          }}, {"Quit", [&running] {
                              running = false;
                          }},
                           {"Save", [&name, &board, &running] {
                               running = false;
                               saveBoard(name, board);
                           }}}
    };

    editMenu();

    clearScreen();

}


void editBoard(Board &board) {

    std::string fileName;
    searchFile(fileName);

    std::ifstream file(fileName);
    board = Board(file);

    bool running = true;

    while (running) {
        clearScreen();
        board.showBoard();
        showEditMenu(board, running);
    }

}


void createBoard(Board &board) {

    unsigned numLines, numCols;
    readDimensions(numLines, numCols);
    board = Board(numLines, numCols);

    bool running = true;

    while (running) {
        clearScreen();
        board.showBoard();
        showEditMenu(board, running);
    }

}



void saveBoard(std::string &fileName, Board board) {
    readFileName(fileName);
    board.save(fileName);
    const Menu saveMenu{"File saved successfully. Do you want to quit or continue editing?\n",
                        "Invalid choice! ",
                        {{"Main menu", [&board] { showMainMenu(board); }},
                         {"Continue editing", [] { }}}
    };
    saveMenu();
}






