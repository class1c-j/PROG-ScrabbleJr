
#ifndef BOARDBUILDER_BOARD_H
#define BOARDBUILDER_BOARD_H


#include <fstream>
#include <vector>
#include <set>

class Board {
public:
    Board();

    Board(unsigned lines, unsigned cols);

    explicit Board(std::ifstream &file);

    void showBoard();

    void save(const std::string &name);

    void insertWord(const std::string &word, std::pair<char, char>, const char &orientation);

    bool verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir);

    unsigned getnLines() const;

    unsigned getnCols() const;

private:
    unsigned numLines{}, numCols{};
    std::set<std::string> words;
    std::vector<std::vector<char> > board;

    void saveWord(const std::string &word, std::pair<char, char>, const char &orientation);

    std::vector<char> getLine(unsigned line);

    std::vector<char> getCol(unsigned col);

    std::set<std::string> startingPoints;

};


#endif //BOARDBUILDER_BOARD_H
