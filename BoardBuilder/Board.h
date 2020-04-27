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
    Board(std::ifstream &file);
    void showBoard();
    void saveBoard(const std::string& name);
    void insertWord(const std::string &word, std::pair<char, char>, const char &orientation);
    bool verifyWord(const std::string& word, std::pair<char, char> coords, const char& dir);

private:
    unsigned numLines, numCols;
    std::vector<std::string> words;
    std::vector< std::vector<char> > board;
    std::vector<char> getLine(unsigned line);
    std::vector<char> getCol(unsigned col);
};


#endif //BOARDBUILDER_BOARD_H
