#include <iostream>
#include "../CommonFiles/Board.h"
#include "gameIO.h"
#include "Player.h"
#include "Pool.h"
#include "Game.h"

void loadBoard(Board &board);

int main() {
    Player player;

    Board board;
    loadBoard(board);
    Pool pool(board);

    size_t number;
    std::cout << "Number of players ? ";
    std::cin >> number;

    std::pair<char, char> coords{};
    char c{};


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

    Game game(players);

    while (!board.isFinished()) {
        for (Player &p : players) {
            board.showBoard();
            p.showHand();
            readLetter(c);
            readCoordinates(coords, board);
            std::cout << "read\n";
            if (p.isValidMove(c, coords, board)) {
                std::cout << "checked\n";
                p.play(c, coords, board);
                std::cout << "played\n";
            }
            board.showBoard();
            p.showHand();
            readLetter(c);
            readCoordinates(coords, board);
            std::cout << "read\n";
            if (p.isValidMove(c, coords, board)) {
                std::cout << "checked\n";
                p.play(c, coords, board);
                std::cout << "played\n";
            }
            if (!pool.getContent().empty()) {
                pool.dealHand(2, p);
            }
        }
    }

}

void loadBoard(Board &board) {
    std::string boardName;
    searchFile(boardName);
    std::ifstream boardFile(boardName);
    board = Board(boardFile);
}
