#include <iostream>
#include "../CommonFiles/Board.h"
#include "gameIO.h"
#include "Player.h"
#include "Pool.h"
#include "Game.h"
#include "../CommonFiles/Menu.h"
#include "../CommonFiles/utility.h"

void loadBoard(Board &board);

Game setup();

void showPlayMenu(Game &game);

int main() {

    Game game = setup();

    std::cout << "Game set up successfully\n";

    clearScreen();
    game.showBoard();
    game.showOthersHands();
    gotoxy(2, game.getSize() + 4);
    showPlayMenu(game);

    std::cout << "Winner is " << game.getWinner().getName() << " with " << game.getWinner().getScore() << '\n';
}

void showPlayMenu(Game &game) {


    const Menu playMenu{"It's your turn, " + game.getCurrentPlayer().getName() + '\n',
                        "Invalid choice! ",
                        {{"Play", [&game] {game.makeTurn();}},
                         {"Get hint", [&game] {game.giveHint();}},
                         {"Show scores", [&game] {game.showLeaderboard(); showPlayMenu(game);}}}
    };

    playMenu();
}

Game setup() {

    //TODO: input validation, deal with boards that do not have enough tiles

    std::cout << "1\n";

    Board board;
    std::cout << "2\n";
    loadBoard(board);
    std::cout << "3\n";
    Pool pool(board);
    std::cout << "4\n";

    size_t number;
    std::cout << "Number of players ? ";
    std::cin >> number;

    std::vector<Player> players(number);

    for (size_t i = 0; i < number; ++i) {
        std::string temp;
        std::cout << "Player[" << i << "] name ? ";
        std::cin >> temp;
        players.at(i) = Player(temp);
    }

    for (auto &i : players) {
        pool.dealHand(7, i);
    }

    return Game(players, board, pool);

}


void loadBoard(Board &board) {

    //TODO: do not let save boards with numbers in the name
    std::string boardName;
    searchFile(boardName);
    std::ifstream boardFile(boardName);
    board = Board(boardFile);

}