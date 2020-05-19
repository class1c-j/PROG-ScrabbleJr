#include <iostream>
#include <vector>
#include "../CommonFiles/Board.h"
#include "../CommonFiles/Menu.h"
#include "editIO.h"

/**
 * @brief shows a menu with options to select between starting a board or editing an existing one
 * @param board - the board
 * @param running - a flag passed to allow for the menu to stop the programme
 */
void showMainMenu(Board board, bool &running);

void showEditMenu(Board &board, bool &running);

void saveBoard(std::string &fileName, Board board, bool &running);

void createBoard(Board &board);

void editBoard(Board &board);

int main() {

#ifdef _WIN32
    windowsSetup();
#endif

    bool running = true;
    Board board{};

    while (running) {
        showMainMenu(board, running);
    }

}

/**
 * @brief shows a logo with the program's name
 */
void showName() {

    std::cout << "  ___                         _     ___          _   _      _             \n"
                 " | _ )  ___   __ _   _ _   __| |   | _ )  _  _  (_) | |  __| |  ___   _ _ \n"
                 " | _ \\ / _ \\ / _` | | '_| / _` |   | _ \\ | || | | | | | / _` | / -_) | '_|\n"
                 " |___/ \\___/ \\__,_| |_|   \\__,_|   |___/  \\_,_| |_| |_| \\__,_| \\___| |_|  \n"
                 "                                                                          \n";
}

/**
 * @brief shows a menu interface to help the user creating or editing existing boards
 * @param board the board in question
 * @param running whether or not the program is running, passed by reference so that it can be changed
 */
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

/**
 * @brief show a menu interface that helps the user create or edit their board
 * @param board the board to be edited
 * @param running whether the program is running or not, passed by reference so that the function can change it
 */
void showEditMenu(Board &board, bool &running) {

    char l{}, c{}, orientation{};
    auto coords = std::make_pair(l, c);

    std::string word{}, name{};


    const Menu editMenu{"This is edit mode. Choose from the options below to create your board.\n",
                        "Invalid choice! ",
                        {{"Add new word", [&board, &coords, &orientation, &word] {
                            readWord(word);
                            readCoordinates(coords, board);
                            readOrientation(orientation);
                            if (board.verifyWord(word, coords, orientation)) {
                                board.insertWord(word, coords, orientation);
                            }
                        }}, {"Erase existing word", [&board, &coords, &orientation] {
                            readCoordinates(coords, board);
                            readOrientation(orientation);
                            board.removeWord(coords, orientation);
                        }}, {"Save current board", [&name, &board, &running] {
                            saveBoard(name, board, running);
                        }},
                         {"Quit to main menu", [&running] {
                             running = false;
                         }}}
    };

    editMenu();

}

/**
 * @brief function to edit an existing board
 * @param board the board
 */
void editBoard(Board &board) {

    std::string fileName;
    searchBoard(fileName);
    std::ifstream file(fileName);
    board = Board(file);

    board.readDictionary();

    bool running = true;

    while (running) {
        clearScreen();
        board.show(std::cout);
        if (!board._error.empty()) {  // show any error that might exist
            printMessage(board._error, 1);
            board._error = "";
        }
        std::cout << "\n";
        showEditMenu(board, running);
    }

}


/**
 * @brief function to create a board from scratch
 * @param board the board
 */
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
        std::cout << "\n";
        showEditMenu(board, running);
    }
}


/**
 * @brief function to save a board with a menu that will assist the user in doing so
 * @param fileName the name of the file to save
 * @param board the board to save
 * @param running whether or not the the program is running passed by referenced so that it can be changed by the function
 */
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
