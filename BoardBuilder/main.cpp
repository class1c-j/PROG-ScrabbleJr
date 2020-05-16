#include <iostream>
#include <fstream>
#include <vector>
#include "../CommonFiles/Board.h"
#include "../CommonFiles/Menu.h"
#include "editIO.h"
#include "../CommonFiles/utility.h"

/**
 * @brief shows a menu with options to select between starting a board or editing an existing one
 * @param board - the board
 * @param running - a flag passed to allow for the menu to stop the programme
 */
void showMainMenu(Board board, bool &running);


/**
 * @brief
 * @param board
 * @param running
 */
void showEditMenu(Board &board, bool &running);

void saveBoard(std::string &fileName, Board board, bool &running);

void createBoard(Board &board);

void editBoard(Board &board);

int main() {

    bool running = true;
    Board board{};

    while (running) {
        showMainMenu(board, running);
    }

}

void showName() {

    std::cout << "  ___                         _     ___          _   _      _             \n"
                 " | _ )  ___   __ _   _ _   __| |   | _ )  _  _  (_) | |  __| |  ___   _ _ \n"
                 " | _ \\ / _ \\ / _` | | '_| / _` |   | _ \\ | || | | | | | / _` | / -_) | '_|\n"
                 " |___/ \\___/ \\__,_| |_|   \\__,_|   |___/  \\_,_| |_| |_| \\__,_| \\___| |_|  \n"
                 "                                                                          \n";
}

void showMainMenu(Board board, bool &running) {

    clearScreen();

    showName();
    const Menu mainMenu{"Welcome to the Board Builder Programme. Here you can build boards for ScrabbleJr.\n",
                        "Invalid choice! ",
                        {{"Create a new board", [&board] {
                            createBoard(board);
                        }}, {"Edit existing board", [&board] {
                            editBoard(board);
                        }}, {"Quit board builder", [&running] {
                            running = false;
                        }}}
    };
    mainMenu();

}

void showEditMenu(Board &board, bool &running) {

    char l{}, c{}, orientation{};
    auto coords = std::make_pair(l, c);

    std::string word{}, name{};


    const Menu editMenu{"This is edit mode. Choose from the options below to create your board.\n",
                        "Invalid choice! ",
                        {{"Insert word", [&board, &coords, &orientation, &word] {
                            readWord(word);
                            readCoordinates(coords, board);
                            readOrientation(orientation);
                            if (board.verifyWord(word, coords, orientation)) {
                                board.insertWord(word, coords, orientation);
                            }
                        }}, {"Remove word", [&board, &coords, &orientation] {
                            readCoordinates(coords, board);
                            readOrientation(orientation);
                            board.removeWord(coords, orientation);
                        }}, {"Save", [&name, &board, &running] {
                            saveBoard(name, board, running);
                        }},
                         {"Quit", [&running] {
                             running = false;
                         }}}
    };

    editMenu();

}


void editBoard(Board &board) {

    std::string fileName;
    searchFile(fileName);
    std::ifstream file(fileName);
    board = Board(file);

    board.readDictionary();

    bool running = true;

    while (running) {
        clearScreen();
        board.show(std::cout);
        showEditMenu(board, running);
    }

}


void createBoard(Board &board) {

    unsigned numLines, numCols;
    readDimensions(numLines, numCols);
    board = Board(numLines, numCols);
    board.readDictionary();

    bool running = true;

    while (running) {
        clearScreen();
        board.show(std::cout);
        if (!board._error.empty()) {  // show any error that might exist
            printMessage(board._error, 1);
            board._error = "";
        }
        showEditMenu(board, running);
    }
}


void saveBoard(std::string &fileName, Board board, bool &running) {
    printMessage(board._error, 1);
    readFileName(fileName);
    std::string filePath = "../UserBoards/" + fileName;
    board.save(filePath);
    const Menu saveMenu{"File saved successfully. Do you want to quit or continue editing?\n",
                        "Invalid choice! ",
                        {{"Main menu", [&running] { running = false;}},
                         {"Continue editing", [] {}}}
    };
    saveMenu();
}
