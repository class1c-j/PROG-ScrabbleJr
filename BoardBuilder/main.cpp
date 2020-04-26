#include <iostream>
#include "Board.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    //testing
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
    return 0;
}
