
#pragma once

#include <fstream>
#include <vector>
#include <set>

class Board {
public:
    Board();

    Board(unsigned lines, unsigned cols);

    explicit Board(std::ifstream &file);

    void readDictionary();

    void save(const std::string &name);

    void showBoard();

    void insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation);

    void removeWord(std::pair<char, char>, char dir);

    bool verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir);

    unsigned getnLines() const;

    unsigned getnCols() const;

    std::string getWord(std::pair<char, char> coords, const char &dir);

    std::vector<std::string> getWordList();

    std::string error{};

    std::vector<std::string> dictWords;

private:

    void saveWord(const std::string &word, std::pair<char, char>, const char &orientation);

    void unsaveWord(std::pair<char, char> coords, const char &dir);

    std::vector<char> getLine(unsigned line);

    std::vector<char> getCol(unsigned col);

    bool checkCol(std::vector<char> col);

    bool checkLine(std::vector<char> line);

    bool isInDictionary(const std::string &word);

    std::set<std::string> startingPoints;

    std::vector<std::string> errors = {
            "ERROR: Out of board limits\n", "ERROR: Bad intersection\n", "ERROR: Word already next to this word\n",
            "ERROR: Word not on dictionary\n", "ERROR: No word to be removed\n", "ERROR: Dictionary not found."
    };

    unsigned numLines{}, numCols{};

    std::set<std::string> words;

    std::vector<std::vector<char> > board;

};