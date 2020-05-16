
#ifndef PROGT2_BOARD_H
#define PROGT2_BOARD_H

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

    void show(std::ostream& stream);

    void insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation);

    void removeWord(std::pair<char, char>, char dir);

    bool verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir);

    size_t getNumLines() const;

    size_t getNumCols() const;

    std::string getWordToRemove(std::pair<char, char> coords, const char &dir);

    std::string _error{};

    std::vector<std::vector<char> > getContent();

    bool isPlayed(std::pair<char, char> coords);

    void setPlayed(std::pair<char, char> coords);

    std::vector<char> getLine(size_t line);

    std::vector<char> getCol(size_t col);

    std::vector<std::pair<char, char>> getWordsInPointStart(std::pair<char, char> coords);

    bool isFinished();

    int finishedWord(std::pair<char, char> coords);

    void saveAllWords();

    std::vector<char> getBoardLetters();

    unsigned maxPlayersAllowed();

    std::pair<char, char> getFirstNotPlayedH(std::pair<char, char> coords);

    std::pair<char, char> getFirstNotPlayedV(std::pair<char, char> coords);

private:

    void saveWord(const std::string &word, std::pair<char, char>, const char &orientation);

    bool checkCol(std::vector<char> col, std::pair<char, char> coords, const std::string& word) const;

    bool checkLine(std::vector<char> nextLine, std::pair<char, char> coords, const std::string& word) const;

    bool isInDictionary(const std::string &word);
    
    bool checkNewWord(std::vector<char> col, const char& orientation, const char newDir);

    void deleteTemporaryWord(std::pair<char, char> coords, const char& orientation, std::vector<char> col);

    bool isInBoardLimits(const std::string& word, std::pair<char, char> coords, const char& dir) const;

    bool badIntersection(const std::string& word, std::pair<char, char> coords, const char& dir);

    bool checkWordLimits(const std::string& word, std::pair<char, char> coords, const char& dir);

    bool checkWordNeighborhood(const std::string& word, std::pair<char, char> coords, const char& dir);

    bool checkNewWords(const std::string& word, std::pair<char, char> coords, const char& dir);

    std::string searchWord(std::pair<char, char> coords, const char &dir);

    std::vector<char> _allLetters;

    std::set<std::string> _startingPoints;

    std::vector<std::string> _errors = {
            "ERROR: Out of board limits\n", "ERROR: Bad intersection\n", "ERROR: Word already next to this word\n",
            "ERROR: Word not on dictionary\n", "ERROR: No word to be removed\n", "ERROR: Dictionary not found.",
            "WARNING: Number of existing _letters is not enough to play a game with the maximum number of players\n",
            "ERROR: Board does not have enough tiles for the desired number of players\n"
    };

    size_t _numLines{}, _numCols{};

    std::set<std::string> _wordsInfo;

    std::vector<std::string> _dictionary;

    std::vector<std::vector<char> > _board;

    std::vector<std::vector<char> > _played;

    std::string _playedColour = "\033[48;2;140;45;25m";

    std::string _noColour = "\033[39;49m";
};

#endif //PROGT2_BOARD_H
