
#ifndef PROGT2_BOARD_H
#define PROGT2_BOARD_H

#define _ENGINEERING "\033[48;2;140;45;25m"
#define _DEFAULT "\033[39;49m"

#include <fstream>
#include <vector>
#include <set>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "../CommonFiles/utility.h"

class Board {
public:

    // constructors

    Board();

    Board(unsigned lines, unsigned cols);

    explicit Board(std::ifstream &file);

    // get (and other methods that transmit information about the board's state)

    size_t getNumberLines() const;

    size_t getNumberCols() const;

    char getLetter(std::pair<char, char> coords);

    bool isFinished();

    int getNumberOfCompletedWordsByPlay(std::pair<char, char> coords);

    bool isPlayed(std::pair<char, char> coords);

    std::pair<char, char> getFirstFreeHorizontal(std::pair<char, char> coords);

    std::pair<char, char> getFirstFreeVertical(std::pair<char, char> coords);

    std::vector<std::pair<char, char>> getStartOfWordsInPoint(std::pair<char, char> coords);

    std::vector<char> getAllLetters();

    // set (and other methods that alter the board's state)

    void setPlayed(std::pair<char, char> coords);

    void insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation);

    void removeWord(std::pair<char, char>, char dir);

    void readDictionary();

    // show (and other methods related to representations of the board)

    void save(const std::string &name);

    void show(std::ostream& stream);

    // processing

    bool verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir);

    // data

    std::string _error{};

private:

    // get (and other methods that transmit information about the board's state)

    std::vector<char> getLine(size_t line);

    std::vector<char> getCol(size_t column);

    std::string getWordToRemove(std::pair<char, char> coords, const char &dir);

    std::string searchWord(std::pair<char, char> coords, const char &direction);

    // set (and other methods that alter the board's state)

    void saveAllWords();

    void saveWord(const std::string &word, std::pair<char, char>, const char &orientation);

    // processing

    bool isValidVerticalNeighborhood(std::vector<char> col, std::pair<char, char> coords, const std::string& word) const;

    bool isValidHorizontalNeighborhood(std::vector<char> line, std::pair<char, char> coords, const std::string& word) const;

    bool isInDictionary(const std::string &word);
    
    bool isValidAddition(std::vector<char> sequence, char newWordDirection);

    void removeTemporaryWord(std::pair<char, char> coords, const char& dir, std::vector<char> sequence);

    bool isInBoardLimits(const std::string& word, std::pair<char, char> coords, const char& dir) const;

    bool badIntersection(const std::string& word, std::pair<char, char> coords, const char& dir);

    bool validWordLimits(const std::string& word, std::pair<char, char> coords, const char& dir);

    bool validWordNeighborhood(const std::string& word, std::pair<char, char> coords, const char& dir);

    bool causesValidWordAdditions(const std::string& word, std::pair<char, char> coords, const char& dir);

    // data

    std::vector<char> m_allLetters;

    std::set<std::string> m_startingPoints;

    std::vector<std::string> m_errors = {
            "ERROR: Out of board limits\n", "ERROR: Bad intersection\n", "ERROR: Word already next to this word\n",
            "ERROR: Word not on dictionary\n", "ERROR: No word to be removed\n", "ERROR: Dictionary not found.",
            "WARNING: Number of existing _letters is not enough to play a game with the maximum number of players\n",
            "ERROR: Board does not have enough tiles for the desired number of players\n"
    };

    size_t m_numLines{}, m_numCols{};

    std::set<std::string> m_wordsInfo;

    std::vector<std::string> m_dictionary;

    std::vector<std::vector<char> > m_board;

    std::vector<std::vector<char> > m_played;

};

#endif //PROGT2_BOARD_H
