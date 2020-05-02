
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

    void insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation);

    bool verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir);

    void removeWord(std::pair<char, char>, char dir);

    unsigned getnLines() const;

    unsigned getnCols() const;

    std::string getWord(std::pair<char, char> coords, const char &dir);

    void unsaveWord(std::pair<char, char> coords, const char &dir);

    std::string error{};
    
    void file();

    std::vector<std::string> vFile;

private:
    unsigned numLines{}, numCols{};
    std::set<std::string> words;
    std::vector<std::vector<char> > board;

    void saveWord(const std::string &word, std::pair<char, char>, const char &orientation);

    std::vector<char> getLine(unsigned line);

    std::vector<char> getCol(unsigned col);

    std::set<std::string> startingPoints;

    std::vector<std::string> errors = {
            "ERROR: Out of board limits\n", "ERROR: Bad intersection\n", "ERROR: Word already next to this word\n",
            "ERROR: Word is not in the dictionary\n"
    };
    bool wordInFile(const std::string& word);
    
    bool checkLine(std::vector<char> col);
    
    bool checkCol(std::vector<char> nextLine);
};


#endif //BOARDBUILDER_BOARD_H
