
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "../CommonFiles/utility.h"
#include "Board.h"

Board::Board() = default;

Board::Board(unsigned rows, unsigned cols)
        : _numLines(rows), _numCols(cols) {

    // resize the board vector according to the specified dimensions
    _board.resize(_numLines);
    _played.resize(_numLines);
    for (auto &i : _board) {
        i.resize(_numCols);
    }
    for (auto &i : _played) {
        i.resize(_numCols);
    }

    // initialize it with '0' in every position to mark that there are no pieces placed
    for (size_t i = 0; i < _numLines; i++) {
        for (size_t j = 0; j < _numCols; j++) {
            _board.at(i).at(j) = '0';
            _played.at(i).at(j) = '0';
        }
    }

}

Board::Board(std::ifstream &f_in) {

    // read dimensions
    std::string line;
    getline(f_in, line);
    std::istringstream s_in1(line);
    char sep{};
    s_in1 >> _numLines >> sep >> _numCols;

    while (std::getline(f_in, line)) {
        if (line.empty()) break;
        _wordsInfo.insert(line);
    }

    _board.resize(_numLines);
    _played.resize(_numLines);
    for (auto &i : _board) {
        i.resize(_numCols);
    }
    for (auto &i : _played) {
        i.resize(_numCols);
    }

    // initialize it with '0' in every position to mark that there are no words placed
    for (size_t i = 0; i < _numLines; i++) {
        for (size_t j = 0; j < _numCols; j++) {
            _played.at(i).at(j) = '0';
            _board.at(i).at(j) = '0';
        }
    }

    // read words
    for (auto &i : _wordsInfo) {
        std::stringstream str_stream(i);
        char x{}, y{}, d{};
        std::string word;
        str_stream >> x >> y >> d >> word;
        insertWord(word, {x - 65, y - 97}, d);
    }
}


/**
 * @brief load all the words from the provided dictionary into a vector
 */
void Board::readDictionary() {

    std::string word;
    std::ifstream dict("../BoardBuilder/WORDS.TXT");

    if (dict.is_open()) {

        while (dict >> word) {
            _dictionary.push_back(word);
        }

        dict.close();

    } else {
        _error = _errors.at(5);
    }
}


/**
 * @brief save the board in a file
 * @param name - the name of the file
 */
void Board::save(const std::string &name) {

    saveAllWords();

    std::ofstream f_out(name);

    f_out << _numLines << " x " << _numCols << '\n';
    for (const std::string &word : _wordsInfo) {
        if (word.at(3) == 'H')
            f_out << word << '\n';
    }

    for (const std::string &word : _wordsInfo) {
        if (word.at(3) == 'V')
            f_out << word << '\n';
    }

    show(f_out);
}


/**
 * @brief pass the board to a ostream allowing it to "show" it in the terminal or in a file
 * @param stream - the stream used
 */
void Board::show(std::ostream &stream) {

    const size_t W = 1;  // edit to change how close together the _letters are in the board

    // column _letters
    stream << "\n    ";
    for (size_t i = 0; i < _numCols; i++) {
        stream << std::left << std::setw(W) << char(i + 97) << " ";
    }

    stream << '\n';
    stream << std::setfill('-') << std::setw(_numCols * W + _numCols + 4) << std::left << "    " << "\n";

    for (unsigned i = 0; i < _numLines; i++) {

        stream << std::setfill(' ') << std::right << std::setw(2) << char(i + 65) << " |"; // uppercase _letters

        for (size_t j = 0; j < _numCols; j++) {
            if (_played.at(i).at(j) == '1') {
                _board.at(i).at(j) == '0' ? stream << std::left << std::setw(W + 1) << " " : stream << std::left
                << std::setw(W) << _playedColour << (char) toupper(_board.at(i).at(j)) << _noColour << ' ';
            } else {
                _board.at(i).at(j) == '0' ? stream << std::left << std::setw(W + 1) << " " : stream << std::left
                << std::setw(W) << (char) toupper(_board.at(i).at(j)) << ' ';
            }

        }
        stream << '\n';
    }
}


/**
 * @brief inserts a new word in the board
 * @param word - the word to insert
 * @param coords - the starting coordinates
 * @param orientation - the desired direction (vertical or horizontal)
 */
void Board::insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation) {

    if (orientation == 'V') {
        for (size_t i = coords.first; i < coords.first + word.length(); i++) {
            _board.at(i).at(coords.second) = word.at(i - coords.first);
        }
    } else if (orientation == 'H') {
        for (size_t i = coords.second; i < coords.second + word.length(); i++) {
            _board.at(coords.first).at(i) = word.at(i - coords.second);
        }
    }
}

/**
 * @brief removes a word from the board
 * @param coords - the starting position of the word
 * @param dir - the orientation of the word
 */
void Board::removeWord(std::pair<char, char> coords, const char dir) {
    std::string word = getWordToRemove(coords, dir);
    unsigned int brdL = coords.first, brdC = coords.second;

    if (!word.empty()) {

        if (dir == 'V') {

            for (size_t i = brdL; i < brdL + word.length(); i++) {

                if ((brdC == 0 || _board.at(i).at((brdC) - 1) == '0') &&
                    ((brdC + 1) == _numCols || (_board.at(i).at((brdC) + 1) == '0'))) {
                    _board.at(i).at(brdC) = '0';
                }

            }

        } else if (dir == 'H') {

            for (size_t i = brdC; i < brdC + word.length(); i++) {
                if ((brdL == 0 || _board.at(brdL - 1).at(i) == '0') &&
                    ((brdL + 1) == _numLines || (_board.at(brdL + 1).at(i) == '0'))) {
                    _board.at(brdL).at(i) = '0';
                }
            }

        }
    } else {
        _error = _errors.at(4);  // no word to remove
    }
}


/**
 * @brief
 * @param coords
 * @param dir
 * @param sequence
 */
void Board::deleteWord(std::pair<char, char> coords, const char &dir, std::vector<char> sequence) {

    size_t brdL = coords.first, brdC = coords.second;

    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

    if (dir == 'H') {

        for (size_t i = 0; i < _numCols; i++) {
            if (sequence.at(i) != line.at(i)) {
                _board.at(brdL).at(i) = '0';
            }
        }

    } else if (dir == 'V') {

        for (size_t i = 0; i < _numLines; i++) {
            if (sequence.at(i) != col.at(i)) {
                _board.at(i).at(brdC) = '0';
            }
        }

    }
}


/**
@brief Checks if it is valid to insert the word (entered by the user) in the board
@param word - word entered by the user
@param coords - the coordinates of the word's beginning
@param dir - orientation of the word ('V' or 'H')
@return false if word or word's place are not valid, otherwise true
*/
bool Board::verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir) {

    bool check = true;

    if (!isInDictionary(word)) {

        _error = _errors.at(3); // word is not in the dictionary
        check = false;

    } else if (!isInBoardLimits(word, coords, dir)) {

        _error = _errors.at(0);  // crosses board limits
        check = false;

    } else if (badIntersection(word, coords, dir)) {

        _error = _errors.at(1); // causes bad intersection
        check = false;

    } else if ((!checkWordLimits(word, coords, dir)) || (!checkWordNeighborhood(word, coords, dir)) ||
               (!checkNewWords(word, coords, dir))) {

        _error = _errors.at(2);  // touches existing word
        check = false;

    }
    return check;
}


/**
 * @brief checks if a word does not cross the board limits
 * @param word - the word
 * @param coords - the start of the word
 * @param dir - the direction of the word
 * @return true if the word stays withing the board limits, false otherwise
 */
bool Board::isInBoardLimits(const std::string &word, std::pair<char, char> coords, const char &dir) const {

    char startCol = coords.second, startLine = coords.first;

    return !((dir == 'H' && (_numCols - startCol) < word.size()) || (dir == 'V' && (_numLines - startLine) < word.size()));
}


/**
 * @brief check if the insertion of a word causes a bad word intersection on the board
 * @param word - the inserted word
 * @param coords - the starting position of the inserted word
 * @param dir - the direction of the word (horizontal (H) or vertical (V))
 * @return true if the word causes a problematic intersection, false otherwise
 */
bool Board::badIntersection(const std::string &word, std::pair<char, char> coords, const char &dir) {

    size_t wLen = word.size(), brdL = coords.first, brdC = coords.second;

    size_t hLimit = (wLen + brdC); //position (column) next to the last horizontal word position
    size_t vLimit = (wLen + brdL); //position (line) next to the last vertical word position

    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

    bool invalidIntersection = false;

    if (dir == 'H') {

        for (size_t i = brdC; i < hLimit; i++) {
            if ((line.at(i) != '0') && (word.at(i - brdC) != line.at(i))) {
                invalidIntersection = true;
                break;
            }
        }

    } else if (dir == 'V') {

        for (unsigned i = brdL; i < vLimit; i++) {
            if ((col.at(i) != '0') && (word.at(i - brdL) != col.at(i))) {
                invalidIntersection = true;
                break;
            }
        }

    }

    return invalidIntersection;

}


/**
 * @brief
 * @param word
 * @param coords
 * @param dir
 * @return
 */
bool Board::checkWordLimits(const std::string &word, std::pair<char, char> coords, const char &dir) {

    size_t wLen = word.size(), brdL = coords.first, brdC = coords.second;
    size_t hLimit = (wLen + brdC);
    size_t vLimit = (wLen + brdL);
    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

    if (dir == 'H' && (brdC != 0 || (hLimit) != _numCols)) {

        if ((brdC == 0 && line.at(hLimit) != '0') || ((hLimit) == _numCols && line.at(brdC - 1) != '0')
            || ((brdC != 0 && (hLimit) != _numCols) && (line.at(hLimit) != '0' || line.at(brdC - 1) != '0'))) {

            insertWord(word, coords, dir);
            std::vector<char> newLine = getLine(brdL);

            if (!checkNewWord(newLine, dir, 0)) {
                deleteWord(coords, dir, line);
                return false;
            }

            deleteWord(coords, dir, line);
        }

    } else if (dir == 'V' && (brdL != 0 || (vLimit) != _numLines)) {

        if ((brdL == 0 && col.at(vLimit) != '0') || (vLimit == _numLines && col.at(brdL - 1) != '0')
            || ((brdL != 0 && (vLimit) != _numLines) && (col.at(vLimit) != '0' || col.at(brdL - 1) != '0'))) {

            insertWord(word, coords, dir);
            std::vector<char> newCol = getCol(brdC);
            if (!checkNewWord(newCol, dir, 0)) {
                deleteWord(coords, dir, col);
                return false;
            }
            deleteWord(coords, dir, col);
        }

    }

    return true;
}


/**
 * @brief
 * @param word
 * @param coords
 * @param dir
 * @return
 */
bool Board::checkWordNeighborhood(const std::string &word, std::pair<char, char> coords, const char &dir) {

    size_t brdL = coords.first, brdC = coords.second;
    bool check = true;

    if (dir == 'H' && ((brdL != 0 || (brdL + 1) != _numLines))) {

        if ((brdL + 1) != _numLines) {

            std::vector<char> nextLine = getLine(brdL + 1);
            check = checkLine(nextLine, coords, word);   //if wordLine != lastLine checks the inferior neighborhood
        }
        if (brdL != 0 && check) {

            std::vector<char> prevLine = getLine(brdL - 1);
            check = checkLine(prevLine, coords, word);  //if wordLine != firstLine just checks the superior neighborhood
        }

    } else if (dir == 'V' && (brdC != 0 || ((brdC + 1) != _numCols))) {

        if ((brdC + 1) != _numCols) {
            std::vector<char> nextCol = getCol(brdC + 1);
            check = checkCol(nextCol, coords, word);    //if wordColumn != lastColumn checks the right neighborhood
        }
        if (brdC != 0 && check) {
            std::vector<char> prevCol = getCol(brdC - 1);
            check = checkCol(prevCol, coords, word);    //if wordColumn != firsColumn checks the left neighborhood
        }
    }

    return check;
}


/**
 * @brief
 * @param word
 * @param coords
 * @param dir
 * @return
 */
bool Board::checkNewWords(const std::string &word, std::pair<char, char> coords, const char &dir) {

    //checks if adding the new word an existing word is updated to a larger word and if this last is valid

    size_t wLen = word.size(), brdL = coords.first, brdC = coords.second;
    size_t hLimit = (wLen + brdC);
    size_t vLimit = (wLen + brdL);

    if (dir == 'H') {
        for (size_t i = brdC; i < hLimit; i++) {
            std::vector<char> col = getCol(i);
            char saveBoardPos = col.at(brdL); //this char saves the board in a certain position
            col.at(brdL) = word.at(i - brdC);

            if (!checkNewWord(col, dir, 1)) {
                col.at(brdC) = saveBoardPos;
                return false;
            }
            col.at(brdC) = saveBoardPos;
        }
    } else if (dir == 'V') {
        for (size_t i = brdL; i < vLimit; i++) {
            std::vector<char> line = getLine(i);
            char saveBoardPos = line.at(brdC);   //this char saves the board in a certain position
            line.at(brdC) = word.at(i - brdL);

            if (!checkNewWord(line, dir, 1)) {
                line.at(brdC) = saveBoardPos;
                return false;
            }
            line.at(brdC) = saveBoardPos;
        }
    }
    return true;
}


/**
 * @brief get the number of lines the board has
 * @return number of lines
 */
size_t Board::getNumLines() const {

    return _numLines;
}


/**
 * @brief gets the number of columns the board has
 * @return the number of columns
 */
size_t Board::getNumCols() const {
    return _numCols;
}


/**
 * @brief searches the board for all the letters of the word selected to remove
 * @param coords the starting point of the word
 * @param dir the direction of the word
 * @return a string with the word
 */
std::string Board::getWordToRemove(std::pair<char, char> coords, const char &dir) {

    unsigned int startingLine = coords.first, startingColumn = coords.second;
    std::string word{};

    if (dir == 'V') {

        std::vector<char> column = getCol(startingColumn);
        size_t i = startingLine;

        while (i < _numLines && column.at(i) != '0') {

            if (isalpha(column.at(i))) word += column.at(i);
            i++;
        }

    } else if (dir == 'H') {

        std::vector<char> line = getLine(startingLine);
        size_t i = startingColumn;

        while (i < _numCols && line.at(i) != '0') {

            if (isalpha(line.at(i))) word += line.at(i);
            i++;
        }

    }

    return word;
}


/**
 * @brief saves a word in the format "Lc D word" into a board's vector to later be able to generate the file
 * @param word - the word
 * @param coords - the starting point of the word
 * @param dir - the direction
 */
void Board::saveWord(const std::string &word, std::pair<char, char> coords, const char &dir) {

    std::string pos = std::string() + (char) (coords.first + 65) + (char) (coords.second + 97);
    std::string info = pos + ' ' + dir + ' ' + word;
    _startingPoints.insert(pos);
    _wordsInfo.insert(info);

}


/**
 * @brief searches the board for a word
 * @param coords - the starting point of the word
 * @param dir - the direction of the word
 * @return if there is a word matching the criteria, it is returned, otherwise returns an empty string
 */
std::string Board::searchWord(std::pair<char, char> coords, const char &dir) {

    std::string pos = std::string() + (char) (coords.first + 65) + (char) (coords.second + 97);

    for (const auto &i : _wordsInfo) {

        if (i.substr(0, 4) == pos + ' ' + dir) {

            return i.substr(5);
        }

    }

    return std::string();
}


/**
 * @brief gets the whole line in a vector
 * @param line - the number of the line in ther board
 * @return the line
 */
std::vector<char> Board::getLine(size_t line) {

    return _board.at(line);
}


/**
 * @brief gets the whole column in a vector
 * @param col - the number of the column in the board
 * @return the column
 */
std::vector<char> Board::getCol(size_t col) {

    std::vector<char> column;

    column.reserve(getNumLines());

    for (size_t i = 0; i < getNumLines(); i++) {
        column.push_back(_board.at(i).at(col));
    }

    return column;

}


/**
 * @brief
 * @param line
 * @param coords
 * @param word
 * @return
 */
bool Board::checkLine(std::vector<char> line, std::pair<char, char> coords, const std::string &word) const {
    unsigned int wLen = word.size(), brdC = coords.second;
    size_t hLimit = (wLen + brdC);

    for (size_t i = brdC - 1; i <= hLimit; i++) {
        std::string newWord;
        while (i < _numCols && i >= 0 && i <= hLimit && line.at(i) != '0') {
            newWord += line.at(i);
            if (newWord.size() >= 2) {
                return false;
            }
            i++;
        }
    }
    return true;
}


/**
 * @brief
 * @param col
 * @param coords
 * @param word
 * @return
 */
bool Board::checkCol(std::vector<char> col, std::pair<char, char> coords, const std::string &word) const {
    unsigned int wLen = word.size(), brdL = coords.first;
    size_t vLimit = (wLen + brdL);

    for (size_t i = brdL - 1; i <= vLimit; i++) {
        std::string newWord;
        while (i >= 0 && i < _numLines && i <= vLimit && col.at(i) != '0') {
            newWord += col.at(i);
            if (newWord.size() >= 2) {
                return false;
            }
            i++;
        }
    }
    return true;
}


/**
 * @brief
 * @param sequence
 * @param dir
 * @param flag
 * @return
 */
bool Board::checkNewWord(std::vector<char> sequence, const char &dir, unsigned int flag) {

    int num{};
    //Não fiques Noddy com estas flags; basta colocar um comentário a explicar qual a finalidade delas e acho que não há problema;
    if ((dir == 'V' && flag == 1) || (dir == 'H' && flag == 0)) {
        num = _numCols;
    } else if ((dir == 'H' && flag == 1) || (dir == 'V' && flag == 0)) {
        num = _numLines;
    }

    for (size_t j = 0; j < num; j++) {
        std::string newWord;
        while (sequence.at(j) != '0') {
            newWord += sequence.at(j);
            if (j == num - 1) {
                break;
            }
            j++;
        }
        if (newWord.size() > 1) { // word must have more than one character
            if (!(isInDictionary(newWord))) {
                return false;
            }
        }
    }
    return true;
}


/**
 * @brief scans the board and saves all of it's words
 */
void Board::saveAllWords() {

    //get all words that are vertical
    for (size_t k = 0; k < _numCols; k++) {

        std::vector<char> currentColumn = getCol(k);
        size_t startingLine{};

        for (size_t i = 0; i < _numLines; i++) {

            std::string word{};  // temporary string to hold the word found in the current column

            while (currentColumn.at(i) != '0') { // found a word

                if (word.empty()) startingLine = i;

                word += currentColumn.at(i);

                if (i == _numLines - 1) break;  // reached the end of the column

                i++;
            }

            if (word.size() > 1) { // word must have more than one character

                auto newCoords = std::make_pair((char) startingLine, (char) k);
                saveWord(word, newCoords, 'V');
            }
        }
    }



    //get all words that are horizontal
    for (size_t k = 0; k < _numLines; k++) {

        std::vector<char> line = getLine(k);
        int startingColumn{};

        for (size_t i = 0; i < _numCols; i++) {

            std::string word;

            while (line.at(i) != '0') {

                if (word.empty()) startingColumn = i;

                word += line.at(i);

                if (i == _numCols - 1) break;

                i++;
            }

            if (word.size() > 1) { // word must have more than one character

                auto newCoords = std::make_pair((char) k, (char) startingColumn);
                saveWord(word, newCoords, 'H');
            }

        }
    }
}


/**
 * @brief gets all the letters in the board
 * @return
 */
std::vector<char> Board::getBoardLetters() {

    for (size_t i = 0; i < _numLines; i++) {

        std::vector<char> line = getLine(i);

        for (size_t j = 0; j < _numCols; j++) {

            if (line.at(j) != '0') {
                _allLetters.push_back(line.at(j));
            }

        }

    }

    if (_allLetters.size() < 28) {

        _error = _errors.at(6);
    }

    return _allLetters;

}


/**
 * @brief uses binary search check if a word is in the dictionary
 * @param word - the word to find
 * @return true if the word is in the dictionaty, false otherwise
 */
bool Board::isInDictionary(const std::string &word) {
    return (std::binary_search(_dictionary.begin(), _dictionary.end(), word));
}


/**
 * @brief place a tile
 * @param coords the coordinates of the place where the tile was placed
 */
void Board::setPlayed(std::pair<char, char> coords) {

    _played.at(coords.first).at(coords.second) = '1';
}


/**
 * @brief check if a place has been played
 * @param coords - the place to check
 * @return true if the place has been played, false otherwise
 */
bool Board::isPlayed(std::pair<char, char> coords) {

    return _played.at(coords.first).at(coords.second) == '1';
}


/**
 * @brief get the board
 * @return the board
 */
std::vector<std::vector<char> > Board::getContent() {
    return _board;
}


/**
 * @brief looks for the starts of the words that pass in a given point
 * @param coords - the coordinates of the given point
 * @return a vector with the coordinates of the start of the words that pass in the point
 */
std::vector<std::pair<char, char> > Board::getWordsInPointStart(std::pair<char, char> coords) {

    std::vector<std::pair<char, char> > result;


    // search horizontally
    std::vector<char> currLine = getLine(coords.first);
    std::pair<char, char> startH{};
    size_t i = coords.second;

    if (i == 0) {  // if the word starts ate the start of the line there's no need to search

        startH = {coords.first, i};

    } else {

        while (currLine.at(i) != '0') {
            i--;
            if (i == 0) break;
        }

        if (currLine.at(i) == '0') i++;
        startH = {coords.first, i};

    }


    if (!searchWord(startH, 'H').empty()) result.push_back(startH); else result.emplace_back(-1, -1);


    // search vertically
    std::vector<char> currCol = getCol((size_t) coords.second);
    std::pair<char, char> startV{};
    size_t j = coords.first;

    if (j == 0) { // if the word starts ate the start of the column there's no need to search

        startV = {j, coords.second};

    } else {

        while (currCol.at(j) != '0') {
            j--;
            if (j == 0) break;
        }

        if (currCol.at(j) == '0') j++;
        startV = {j, coords.second};
    }

    if (!searchWord(startV, 'V').empty()) result.push_back(startV); else result.emplace_back(-1, -1);

    return result;
}


/**
 * @brief checks the board for any uncovered tile
 * @return true if all the places have been played, false otherwise
 */
bool Board::isFinished() {
    for (unsigned i = 0; i < getNumLines(); i++) {
        for (unsigned j = 0; j < getNumCols(); j++) {
            if (_board.at(i).at(j) != '0' && !isPlayed({i, j})) {
                return false;
            }
        }
    }
    return true;
}


/**
 * @brief checks for finished words horizontally and vertically
 * @param coords the coords of the last placed piece
 * @return number of finished words
 */
int Board::finishedWord(std::pair<char, char> coords) {

    bool finishedH = true, finishedV = true;

    // search vertically
    std::pair<char, char> startV = getWordsInPointStart(coords).at(1);
    size_t sizeV = searchWord(startV, 'V').size();

    if (sizeV == 0) finishedV = false;
    size_t endV = startV.first + sizeV;

    for (size_t i = startV.first; i < endV; i++) {

        if (!isPlayed({i, coords.second})) {
            finishedV = false;
            break;
        }

    }


    // search horizontally
    std::pair<char, char> startH = getWordsInPointStart(coords).at(0);
    size_t sizeH = searchWord(startH, 'H').size();

    if (sizeH == 0) finishedH = false;
    size_t endH = startH.second + sizeH;

    for (size_t i = startH.second; i < endH; i++) {

        if (!isPlayed({coords.first, i})) {
            finishedH = false;
            break;
        }

    }

    return int(finishedV) + int(finishedH);

}


/**
 * @brief calculates the minimum amount of players required considering a hand of 7 tiles per player
 * @return the number of players
 */
unsigned Board::maxPlayersAllowed() {

    size_t tiles = getBoardLetters().size();

    if (tiles == 0) return 0;
    else if (tiles < 14) return 1;
    else if (tiles < 21) return 2;
    else if (tiles < 28) return 3;
    else return 4;

}


/**
 * @brief gets the position of the first not played letter of an horizontal word
 * @param coords - the starting coordinates of the word
 * @return the coordinates of the first free tile to play
 */
std::pair<char, char> Board::getFirstNotPlayedH(std::pair<char, char> coords) {

    std::pair<char, char> result{};

    std::pair<char, char> start = getWordsInPointStart(coords).at(0);


    if (start != std::pair<char, char>{-1, -1}) {  // if there is an horizontal  word

        std::string word = getWordToRemove(start, 'H');

        // iterate through the board positions where the word is, looking for the first free spot
        for (size_t i = start.second; i < start.second + word.length(); i++) {

            if (!isPlayed({coords.first, i})) {
                result = {coords.first, i};
                break;
            }

        }

    }

    return result;
}


/**
 * @brief gets the position of the first not played letter of a vertical word
 * @param coords - the starting coordinates of the word
 * @return the coordinates of the first free tile to play
 */
std::pair<char, char> Board::getFirstNotPlayedV(std::pair<char, char> coords) {

    std::pair<char, char> result{};

    std::pair<char, char> start = getWordsInPointStart(coords).at(1);

    if (start != std::pair<char, char>{-1, -1}) {  // if there is a vertical word

        std::string word = getWordToRemove(start, 'V');

        for (size_t i = start.first; i < start.first + word.length(); i++) {

            if (!isPlayed({i, coords.second})) {
                result = {i, coords.second};
                break;
            }

        }

    }

    return result;
}
