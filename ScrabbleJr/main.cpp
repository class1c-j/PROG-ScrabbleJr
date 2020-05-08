#include <iostream>
#include "../CommonFiles/Board.h"
#include "gameIO.h"
#include "Player.h"
#include "Pool.h"
#include "Game.h"
#include "../CommonFiles/Menu.h"

void loadBoard(Board &board);

Game setup();

void takeTurn(Game &game, unsigned playerID);


int main() {

    Game game = setup();
    unsigned currentP = 0;
    while (!game.getBoard().isFinished()) {
        takeTurn(game, currentP);
        currentP = game.nextPlayer();
    }

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

void takeTurn(Game &game, unsigned playerID) {

    Board board = game.getBoard();
    Pool pool = game.getPool();
    Player player = game.getPlayers().at(playerID);

    board.showBoard();
    player.showHand();

    char tile{};
    std::pair<char, char> coords{};

    unsigned possibleTiles = player.playableTiles(board).size();

    if (possibleTiles >= 2) {
        std::cout << "2 or more possible plays\n";
        for (int i = 0; i < 2; i++) {
            while (!player.isValidMove(tile, coords, board)) {
                readLetter(tile);
                readCoordinates(coords, board);
            }
            player.play(tile, coords, board);
            board.showBoard();
            player.showHand();
        }
    } else if (possibleTiles == 1) {
        std::cout << "1 possible play\n";
        while (!player.isValidMove(tile, coords, board)) {
            readLetter(tile);
            readCoordinates(coords, board);
        }
        player.play(tile, coords, board);
        board.showBoard();
        player.showHand();
    } else {
        std::vector<char> hand = player.getHand();
        std::cout << "Choose 2 tiles to exchange\n";
        readLetter(tile);
        auto pos = std::find(hand.begin(), hand.end(), tile);
        hand.erase(pos);

        readLetter(tile);
        auto pos2 = std::find(hand.begin(), hand.end(), tile);
        hand.erase(pos2);

        player.setHand(hand);
        possibleTiles = player.playableTiles(board).size();

        if (possibleTiles >= 2) {
            for (int i = 0; i < 2; i++) {
                while (!player.isValidMove(tile, coords, board)) {
                    readLetter(tile);
                    readCoordinates(coords, board);
                }
                player.play(tile, coords, board);
                board.showBoard();
                player.showHand();
            }
        } else if (possibleTiles == 1) {
            while (!player.isValidMove(tile, coords, board)) {
                readLetter(tile);
                readCoordinates(coords, board);
            }
            player.play(tile, coords, board);
            board.showBoard();
            player.showHand();
        } else {
            std::cout << "Nothing to play, passing";
        }
    }

    game.setBoard(board);
    game.setPlayer(player, playerID);
    game.setPool(pool);
}

void loadBoard(Board &board) {

    std::string boardName;
    searchFile(boardName);
    std::ifstream boardFile(boardName);
    board = Board(boardFile);

}
