
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

    void showBoard(std::ostream& stream);

    void insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation);

    void removeWord(std::pair<char, char>, char dir);

    bool verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir);

    unsigned getnLines() const;

    unsigned getnCols() const;

    std::string getWord(std::pair<char, char> coords, const char &dir);

    std::string error{};

    std::vector<std::vector<char> > getContent();

    std::vector<std::string> dictWords;

    bool isPlayed(std::pair<char, char> coords);

    void setPlayed(std::pair<char, char> coords);

    std::vector<char> getLine(size_t line);

    std::vector<char> getCol(size_t col);

    std::vector<std::pair<char, char>> getWordsInPointStart(std::pair<char, char> coords);

    bool isFinished();

    int finishedWord(std::pair<char, char> coords);

    std::vector<std::string> getBoardWords();

    std::vector<char> getBoardLetters();

    std::vector<char> totalLetters;

    unsigned maxPlayersAllowed();

    std::pair<char, char> getFirstNotPlayedH(std::pair<char, char> coords);

    std::pair<char, char> getFirstNotPlayedV(std::pair<char, char> coords);

private:

    void saveWord(const std::string &word, std::pair<char, char>, const char &orientation);

    bool checkCol(std::vector<char> col, std::pair<char, char> coords, const std::string& word) const;

    bool checkLine(std::vector<char> nextLine, std::pair<char, char> coords, const std::string& word) const;

    bool isInDictionary(const std::string &word);

    std::set<std::string> startingPoints;

    std::vector<std::string> errors = {
            "ERROR: Out of board limits\n", "ERROR: Bad intersection\n", "ERROR: Word already next to this word\n",
            "ERROR: Word not on dictionary\n", "ERROR: No word to be removed\n", "ERROR: Dictionary not found.",
            "WARNING: Number of existing _letters is not enough to play a game with the maximum number of players\n",
            "ERROR: Board does not have enough tiles for the desired number of players\n"
    };

    unsigned numLines{}, numCols{};

    std::set<std::string> words;

    std::vector<std::vector<char> > board;

    std::vector<std::vector<char> > played;

    std::string searchWord(std::pair<char, char> coords, const char &dir);
};

#endif //PROGT2_BOARD_H