#include <iostream>
#include <fstream>
#include <vector>
#include "../CommonFiles/Board.h"
#include "../CommonFiles/Menu.h"
#include "editIO.h"

void showMainMenu(Board board, bool &running);

void showEditMenu(Board &board, bool &running);

void saveBoard(std::string &fileName, Board board, bool &running);

void createBoard(Board &board);

void editBoard(Board &board);

int main() {
    bool running = true;
    Board board = Board();
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
        board.showBoard();
        showEditMenu(board, running);
    }
}


void createBoard(Board &board) {

    unsigned numLines, numCols;
    readDimensions(numLines, numCols);
    board = Board(numLines, numCols);
    board.readDictionary();

    std::cout << "Dic size: " << board.dictWords.size() << '\n';

    bool running = true;

    while (running) {
        clearScreen();
        board.showBoard();
        if (!board.error.empty()) {  // show any error that might exist
            printMessage(board.error, 1);
            board.error = "";
        }
        showEditMenu(board, running);
    }
}


void saveBoard(std::string &fileName, Board board, bool &running) {
    board.getBoardWords();
    board.getBoardLetters();
    printMessage(board.error, 1);
    readFileName(fileName);
    std::string filePath = "../UserBoards/" + fileName;
    board.save(filePath);
    const Menu saveMenu{"File saved successfully. Do you want to quit or continue editing?\n",
                        "Invalid choice! ",
                        {{"Main menu", [&board, &running] { running = false;}},
                         {"Continue editing", [] {}}}
    };
    saveMenu();
}
