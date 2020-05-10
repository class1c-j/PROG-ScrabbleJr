#include <iostream>
#include "../CommonFiles/Board.h"
#include "gameIO.h"
#include "Player.h"
#include "Pool.h"
#include "Game.h"
#include "../CommonFiles/Menu.h"

void loadBoard(Board &board);

Game setup();

int main() {

    Game game = setup();

    game.play();

    std::cout << "Winner is " << game.getWinner().getName() << " with " << game.getWinner().getScore() << '\n';
}

Game setup() {

    Board board;
    loadBoard(board);
    Pool pool(board);

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

    std::string boardName;
    searchFile(boardName);
    std::ifstream boardFile(boardName);
    board = Board(boardFile);

}