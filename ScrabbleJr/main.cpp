#include <iostream>
#include "../CommonFiles/Board.h"
#include "gameIO.h"
#include "Player.h"
#include "Pool.h"
#include "Game.h"
#include "../CommonFiles/Menu.h"

void loadBoard(Board &board);

Game setup();

void takeTurn(Game &game);

void makePlay(Board &board, Pool &pool, Player &player);


int main() {

    Game game = setup();
    while (!game.getBoard().isFinished()) {
        std::cout << "Pool size = " << game.getPool().getContent().size() << '\n';
        takeTurn(game);
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

void makePlay(Board &board, Pool &pool, Player &player) {

    char tile{};
    std::pair<char, char> coords{};

    unsigned possibleTiles = player.playableTiles(board).size();

    if (possibleTiles >= 2) {
        std::cout << "You have at least 2 tiles you can play\n";
        for (int i = 0; i < 2; i ++) {
            while (!player.isValidMove(tile, coords, board)) {
                readLetter(tile);
                readCoordinates(coords, board);
            }
            player.play(tile, coords, board);
            board.showBoard();
            player.showHand();
        }
        if (!pool.isEmpty()) pool.dealHand(2, player);
    } else if (possibleTiles == 1) {
        std::cout << "You only have 1 tile that you can play\n";
        while (!player.isValidMove(tile, coords, board)) {
            readLetter(tile);
            readCoordinates(coords, board);
        }
        player.play(tile, coords, board);
        board.showBoard();
        player.showHand();
        possibleTiles = player.playableTiles(board).size();
        if (possibleTiles != 0) {
            while (!player.isValidMove(tile, coords, board)) {
                readLetter(tile);
                readCoordinates(coords, board);
            }
            player.play(tile, coords, board);
            board.showBoard();
            player.showHand();
            if (!pool.isEmpty()) pool.dealHand(2, player);
        } else {
            if (!pool.isEmpty()) pool.dealHand(1, player);
        }
    } else {
        std::cout << "None of your tiles can be played. Choose 2 to exchange for new ones\n";
        for (int i = 0; i < 2; i ++) {
            readLetter(tile);
            while (true) {
                if (!player.hasTile(tile)) {
                    readLetter(tile);
                } else {
                    tile = '0';
                    break;
                }
            }
            player.removeTile(tile);
            board.showBoard();
            player.showHand();
        }
        if (!pool.isEmpty()) pool.dealHand(2, player);
        if (possibleTiles != 0) {
            makePlay(board, pool, player);
        }
        std::cout << "here4\n";
    }
}

void takeTurn(Game &game) {


    Board board = game.getBoard();
    Pool pool = game.getPool();
    Player player = game.getPlayers().at(game.getCurrentP());

    board.showBoard();
    player.showHand();

    makePlay(board, pool, player);

    game.setBoard(board);
    game.setPlayer(player, game.getCurrentP());
    game.setPool(pool);

    game.setCurrentP(game.nextPlayer());
}


void loadBoard(Board &board) {

    std::string boardName;
    searchFile(boardName);
    std::ifstream boardFile(boardName);
    board = Board(boardFile);

}