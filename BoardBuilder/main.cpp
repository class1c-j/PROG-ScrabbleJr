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
<<<<<<< HEAD
    //Board Board(f1);
    //Board.insertWord("Sorocoffo", {0, 3}, 'V');  //AdV
    //Board.insertWord("Sardou", {2, 6}, 'H');    //CgH
    //Board.insertWord("Canss", {1, 14}, 'V');  //BoV
    //Board.showBoard();
    //Board.saveBoard("test1.txt");

    createBoard();

=======
    Board Board(10, 15);
    Board.insertWord("Sorocoffo", {0, 3}, 'V');  //AdV
    Board.insertWord("Sardou", {2, 6}, 'H');    //CgH
    Board.insertWord("Canss", {1, 14}, 'V');  //BoV
    Board.insertWord("semislindosuwu", { 0, 2 }, 'V'); //ERRO -> tamanho da palavra ultrapssa o tamanho do tabuleiro
    Board.insertWord("Jaszito", { 1, 7 }, 'V'); //BhV
    Board.insertWord("Erro", { 0, 2 }, 'H'); //ERRO -> palavra inserida perturba as palavra já inseridas no tabuleiro
    Board.insertWord("ocanaselindo", { 0, 9 }, 'H'); //ERRO -> tamanho da palavra ultrapssa o tamanho do tabuleiro
    Board.insertWord("uwu", { 1, 11 }, 'V');//ERRO -> palavra inserida perturba as palavra já inseridas no tabuleiro
    Board.insertWord("socuerro", { 8, 2 }, 'H'); //IcH
    Board.showBoard();
>>>>>>> 2f1f47abd84743f9cb35fcc1bc7c84536c8bfa5c
    return 0;
}
