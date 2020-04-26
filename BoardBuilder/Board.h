//
// Created by ritam on 25/04/20.
//

#ifndef BOARDBUILDER_BOARD_H
#define BOARDBUILDER_BOARD_H


#include <fstream>
#include <vector>

class Board {
public:
    Board(unsigned lines, unsigned cols);
    void showBoard();
    void insertWord(const std::string &word, std::pair<char, char>, const char &orientation);
private:
    unsigned numLines, numCols;
    std::vector< std::vector<char> > board;
    std::vector<char> getLine(unsigned line);
    std::vector<char> getCol(unsigned col);
};


#endif //BOARDBUILDER_BOARD_H
