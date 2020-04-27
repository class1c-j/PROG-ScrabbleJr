#include <iostream>
#include <fstream>
#include "Board.h"

void createBoard() {
    // for testing purposes, far from complete!!
    unsigned l, c;
    std::string wrd;
    char li, ci, di;
    std::cout << "Number of lines: "; std::cin >> l;
    std::cout << "Number of columns: "; std::cin >> c;

    Board b(l, c);

    b.showBoard();

    std::cout << "insert word: "; std::cin >> wrd;
    std::cout << "Lc Dir: "; std::cin >> li >> ci >> di;
    std::cout << li-65 << " " << ci-97 << '\n';
    std::cout << b.verifyWord(wrd, {li-65, ci-97}, di);
    b.insertWord(wrd, {li-65, ci-97}, di);
    b.showBoard();
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    //std::ifstream f1("test1.txt");
    //testing
    //Board Board(f1);
    //Board.insertWord("Sorocoffo", {0, 3}, 'V');  //AdV
    //Board.insertWord("Sardou", {2, 6}, 'H');    //CgH
    //Board.insertWord("Canss", {1, 14}, 'V');  //BoV
    //Board.showBoard();
    //Board.saveBoard("test1.txt");

    createBoard();

    return 0;
}
