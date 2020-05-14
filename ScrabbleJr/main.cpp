#include <iostream>
#include "../CommonFiles/Board.h"
#include "gameIO.h"
#include "Player.h"
#include "Pool.h"
#include "Game.h"
#include "../CommonFiles/Menu.h"

void loadBoard(Board &board);

Game setup();

void showName();

void showInstructions();

void showMainMenu(Game &game);

int main() {

    Game game;

    showMainMenu(game);

}

Game setup() {

    Board board;
    loadBoard(board);
    Pool pool(board);

    size_t number;
    readNumberPlayers(number);
    while (number > board.maxPlayersAllowed()) {
        std::cout << "The selected board does not have enough tiles.\n";
        readNumberPlayers(number);
    }

    std::vector<std::string> names = readPlayersNames(number);

    std::vector<Player> players;

    players.reserve(names.size());

    for (const auto &i : names) {
        players.emplace_back(i);
    }

    for (auto &i : players) {
        pool.dealHand(7, i);
    }

    return Game(players, board, pool);

}

void showName() {
    std::cout << "  ___              _    _    _          _      \n"
                 " / __| __ _ _ __ _| |__| |__| |___   _ | |_ _  \n"
                 " \\__ \\/ _| '_/ _` | '_ \\ '_ \\ / -_) | || | '_| \n"
                 " |___/\\__|_| \\__,_|_.__/_.__/_\\___|  \\__/|_|(_)";

    std::cout << "\n\n";
}

void showInstructions() {

    clearScreen();
    showName();
    std::cout << "\n";

    std::cout << "Scrabble Jr is an adaptation of the game popular Scrabble aimed at young children "
                 "learning how to read and write\n\nEach player starts with 7 tiles and their goal is to cover all"
                 "of the board's words with them\nIf, during their turn, the player has 2 tiles that can be placed,"
                 "they place the tiles and\ndraw 2 from the pool.\nIf they only have 1 tile that can be played, they "
                 "place it and draw 1 tile from the pool.\nIn case the player can not play any of their tiles,"
                 "they will choose 2 of their tiles to\nexchange for new\n ones in the pool.\n\nYou can place your tiles"
                 " in the places that have a corresponding letter only if it is the\nfirst open letter in spelling order."
                 "\n\nThe game ends when every word in the board has been finished and the winner is the player\n who "
                 "has the highest score from completing words\n\nPress ENTER to go back to menu ...";

    std::cin.ignore(1);
}


void loadBoard(Board &board) {

    std::string boardName;
    searchFile(boardName);
    std::ifstream boardFile(boardName);
    board = Board(boardFile);

}

void showMainMenu(Game &game) {

    clearScreen();

    bool running = true;

    const Menu mainMenu{"Welcome to Scrabble Jr. Here you can play this game alone or with friends.\n",
                        "Invalid choice! ",
                        {{"New game", [&game] {
                            game = setup();
                            clearScreen();
                            game.showBoard();
                            game.showAllHands();
                            while (!game.isFinished()) {
                                gotoxy(game.getSize() + 30, game.getSize() + 5);
                                game.makePlay();
                                game.nextPlayer();
                            }
                            game.showLeaderboard();
                            std::cin.ignore();
                        }}, {"Show Instructions", [] {
                            showInstructions();
                            std::cin.ignore();
                        }}, {"Quit", [&running] {
                            running = false;
                        }}}
    };

    while (running) {
        clearScreen();
        showName();
        mainMenu();
    }
}