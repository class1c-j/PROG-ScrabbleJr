#include <iostream>
#include "Board.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    //testing
    Board Board(10, 15);
    Board.insertWord("Sorocoffo", {0, 3}, 'V');  //AdV
    Board.insertWord("Sardou", {2, 6}, 'H');    //CgH
    Board.insertWord("Canss", {1, 14}, 'V');  //BoV
    Board.showBoard();

    return 0;
}
