#include <iostream>
#include "Pool.h"
#include "../CommonFiles/Board.h"
#include "../BoardBuilder/editIO.h"

void loadBoard(Board &board);

int main() {
    Board board;
    loadBoard(board);
    board.showBoard();
    Pool pool(board.getWordList());
    pool.shuffle();
    std::cout << std::endl;
    for (const char& i : pool.getContent()) {
        std::cout << i << " ";
    }
}

void loadBoard(Board &board) {
    std::string fileName;
    searchFile(fileName);
    std::cout << '\n';
    std::cout << fileName;
    std::ifstream file(fileName);
    board = Board(file);
}
