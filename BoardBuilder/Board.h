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
private:
    unsigned numLines, numCols;
    std::vector< std::vector<char> > board;
};


#endif //BOARDBUILDER_BOARD_H
