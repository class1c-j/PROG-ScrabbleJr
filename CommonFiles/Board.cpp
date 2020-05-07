#include "Board.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "../CommonFiles/utility.h"

Board::Board() = default;

Board::Board(unsigned rows, unsigned cols) {
    numLines = rows;
    numCols = cols;

    // resize the board vector according to the specified dimensions
    board.resize(numLines);
    played.resize(numLines);
    for (auto &i : board) {
        i.resize(numCols);
    }
    for (auto &i : played) {
        i.resize(numCols);
    }

    // initialize it with '0' in every position to mark that there are no pieces placed
    for (size_t i = 0; i < numLines; i++) {
        for (size_t j = 0; j < numCols; j++) {
            board.at(i).at(j) = '0';
            played.at(i).at(j) = '0';
        }
    }

}

Board::Board(std::ifstream &f_in) {

    // read dimentions
    std::string line;
    getline(f_in, line);
    std::istringstream s_in1(line);
    char sep;
    s_in1 >> numLines >> sep >> numCols;
    while (std::getline(f_in, line)) {
        words.insert(line);
    }

    board.resize(numLines);
    played.resize(numLines);
    for (auto &i : board) {
        i.resize(numCols);
    }
    for (auto &i : played) {
        i.resize(numCols);
    }

    // initialize it with '0' in every position to mark that there are no words placed
    for (size_t i = 0; i < numLines; i++) {
        for (size_t j = 0; j < numCols; j++) {
            played.at(i).at(j) = '0';
            board.at(i).at(j) = '0';
        }
    }

    // read words
    for (auto &i : words) {
        std::stringstream str_stream(i);
        char x, y, d;
        std::string word;
        str_stream >> x >> y >> d >> word;
        insertWord(word, {x - 65, y - 97}, d);
    }
}

void Board::readDictionary() {
    std::string word;
    std::ifstream dict("../BoardBuilder/WORDS.TXT");

    if (dict.is_open()) {
        while (dict >> word) {
            dictWords.push_back(word);
        }
        dict.close();
    } else {
        error = errors.at(5);
    }
}

void Board::save(const std::string &name) {

    std::ofstream f_out(name);
    f_out << numLines << " x " << numCols << '\n';
    for (const std::string &word : words) {
        f_out << word << '\n';
    }
}

void Board::showBoard() {

    const int W = 2;  // edit to change how close together the letters are in the board

    // column letters
    std::cout << "\n    ";
    for (int i = 0; i < numCols; i++) {
        std::cout << std::left << std::setw(W) << char(i + 97) << " ";
    }

    std::cout << '\n';
    std::cout << std::setfill('_') << std::setw(numCols * W + numCols + W) << std::left << "    " << "\n";

    for (int i = 0; i < numLines; i++) {

        std::cout << std::setfill(' ') << std::right << std::setw(2) << char(i + 65) << " |";   // uppercase letters

        for (size_t j = 0; j < numCols; j++) {
            if (played.at(i).at(j) == '0') {
                board.at(i).at(j) == '0' ? std::cout << std::left << std::setw(W + 1) << " " : std::cout << std::left
                << std::setw(W) << (char) toupper(board.at(i).at(j)) << ' ';
            } else {
                //setBackgroundColor(1);
                std::cout << "\033[94;103m";
                board.at(i).at(j) == '0' ? std::cout << std::left << std::setw(W + 1) << " " : std::cout << std::left
                << std::setw(W) << (char) toupper(board.at(i).at(j)) << ' ';
                setBackgroundColor(0);
            }

        }
        std::cout << '\n';
    }
}

void Board::insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation) {

    if (orientation == 'V') {
        for (int i = coords.first; i < coords.first + word.length(); i++) {
            board.at(i).at(coords.second) = word.at(i - coords.first);
        }
    } else if (orientation == 'H') {
        for (int i = coords.second; i < coords.second + word.length(); i++) {
            board.at(coords.first).at(i) = word.at(i - coords.second);
        }
    }
}

void Board::removeWord(std::pair<char, char> coords, const char dir) {
    std::string word = getWord(coords, dir);

    if (!word.empty()) {
        if (dir == 'V') {
            for (size_t i = coords.first; i <= coords.first + word.length(); i++) {
                board.at(i).at(coords.second) = 0;
            }
        } else if (dir == 'H') {
            for (size_t i = coords.second; i < coords.second + word.length(); i++) {
                board.at(coords.first).at(i) = 0;
            }
        }
    } else {
        error = errors.at(4);  // no word to remove
    }
}

bool Board::verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir) {

    bool check = true;
    size_t wLen = word.size(), brdL = coords.first, brdC = coords.second;

    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

    std::cout << word << " " << isInDictionary(word) << '\n';

    if (!isInDictionary(word)) {
        error = errors.at(3); // word is not in the dictionary
        check = false;

    } else if (dir == 'H' && (numCols - brdC) < wLen || (dir == 'V' && (numLines - brdL) < wLen)) {
        check = false;
        error = errors.at(0);  // crosses board limits

    } else if (dir == 'H') {

        for (unsigned i = brdC; i < brdC + wLen; i++) {
            if ((line.at(i) != '0') && (word.at(i - brdC) != line.at(i))) {
                error = errors.at(1);  // causes bad intersection
                check = false;
                break;
            }
        }

        if (brdC != 0 || (brdC + wLen) != numCols) {

            // check word's horizontal ends in the cases that they touch the board's boundaries
            if ((brdC == 0 && line.at(brdC + wLen) != '0') || ((brdC + wLen) == numCols && line.at(brdC - 1) != '0')) {
                error = errors.at(2);  // touches existing word
                check = false;
            }

                // check word's horizontal ends in the cases that they don't touch the board's boundaries
            else if ((brdC != 0 && (brdC + wLen) != numCols) &&
                     (line.at(brdC + wLen) != '0' || line.at(brdC - 1) != '0')) {
                error = errors.at(2);  // touches existing word
                check = false;
            }
        }

        // check superior and inferior neighborhoods
        if ((brdL != 0 || (brdL + 1) != numLines) && check) {
            if (brdL == 0) {
                std::vector<char> nextLine = getLine(1);
                check = checkLine(nextLine, coords, word);
            }
            else if ((brdL + 1) == numLines) {
                std::vector<char> prevLine = getLine(brdL - 1);
                check = checkLine(prevLine, coords, word);
            }
            else {
                std::vector<char> nextLine = getLine(brdL + 1);
                check = checkLine(nextLine, coords, word);
                if (check) {
                    std::vector<char> prevLine = getLine(brdL - 1);
                    check = checkLine(prevLine, coords, word);
                }
            }
            if (!check) {
                error = errors.at(2);
            }
        }

    } else if (dir == 'V') {
        for (unsigned i = brdL; i < brdL + wLen; i++) {
            if ((col.at(i) != '0') && (word.at(i - brdL) != col.at(i))) {
                error = errors.at(1);  // causes bad intersection
                check = false;
                break;
            }
        }
        if (brdL != 0 || (brdL + wLen) != numLines) {

            // check word's vertical ends in the cases that they touch the board's boundaries
            if ((brdL == 0 && col.at(brdL + wLen) != '0') || (brdL + wLen == numLines && col.at(brdL - 1) != '0')) {
                error = errors.at(2);  // existing word next to this location
                check = false;
            }
                // check word's vertical ends in the cases that they touch the board's boundaries
            else if ((brdL != 0 && (brdL + wLen) != numLines) &&
                     (col.at(brdL + wLen) != '0' || col.at(brdL - 1) != '0')) {
                error = errors.at(2);  // existing word next to this location
                check = false;
            }
        }

        // check right and left neighborhoods
        if (brdC != 0 || ((brdC + 1) != numCols) && check) {
            if (brdC == 0) {
                std::vector<char> nextCol = getCol(1);
                check = checkCol(nextCol, coords, word);
            }
            else if ((brdC + 1) == numCols) {
                std::vector<char> prevCol = getCol(brdC - 1);
                check = checkCol(prevCol, coords, word);
            }
            else {
                std::vector<char> nextCol = getCol(brdC+1);
                check = checkCol(nextCol, coords, word);
                if (check) {
                    std::vector<char> prevCol = getCol(brdC - 1);
                    check = checkCol(prevCol, coords, word);
                }
            }
            if (!check) {
                error = errors.at(2);
            }
        }
    }
    return check;
}

unsigned Board::getnLines() const {
    return numLines;
}

unsigned Board::getnCols() const {
    return numCols;
}

std::string Board::getWord(std::pair<char, char> coords, const char &dir) {
    char line = coords.first + 65;
    char col = coords.second + 97;
    for (const auto &entry : words) {
        if (line == entry.at(0) && entry.at(1) == col && entry.at(3) == dir) {
            return entry.substr(5);
        }
    }
    return std::string();
}

std::vector<std::string> Board::getWordList() {
    std::vector<std::string> wordList;
    for (const auto &i : words) {
        wordList.push_back(i.substr(5));
    }
    return wordList;
}

void Board::saveWord(const std::string &word, std::pair<char, char> coords, const char &dir) {
    std::string pos = std::string() + (char) (coords.first + 65) + (char) (coords.second + 97);
    std::string info = pos + ' ' + dir + ' ' + word;
    startingPoints.insert(pos);
    words.insert(info);
}

std::vector<char> Board::getLine(unsigned int line) {
    return board.at(line);
}

std::vector<char> Board::getCol(unsigned int col) {
    std::vector<char> column;
    for (auto line : board) {
        column.push_back(line.at(col));
    }
    return column;
}



bool Board::checkLine(std::vector<char> line, std::pair<char, char> coords, const std::string& word) const {
    unsigned int wLen = word.size(), brdC = coords.second;
    int i = brdC;
    while (i <= (brdC + wLen)) {
        std::string newWord;
        if (i >= 0 && i < numCols) {
            while (line.at(i) != '0') {
                newWord += line.at(i);
                if (newWord.size() >= 2) {
                    return false;
                }
                i++;
            }
        }
        i++;
    }
    return true;
}

bool Board::checkCol(std::vector<char> col, std::pair<char, char> coords, const std::string& word) const {
    unsigned int wLen = word.size(), brdL = coords.first;
    int i = brdL-1;
    while (i <= (brdL+wLen)) {
        std::string newWord;
        if (i >= 0 && i < numLines) {
            while (col.at(i) != '0') {
                newWord += col.at(i);
                if (newWord.size() >= 2) {
                    return false;
                }
                i++;
            }
        }
        i++;
    }
    return true;
}

void Board::getBoardWords() {
    //get all words that are vertical
    for (int k = 0; k < numCols; k++) {
        std::vector<char> Col = getCol(k);
        int i = 0, j = 0;
        while (i < numLines) {
            std::string newWord = "";
            while (Col.at(i) != '0') {
                if (newWord == "") {
                    j = i;
                }
                newWord += Col.at(i);
                if (i == numLines - 1) {
                    break;
                }
                else {
                    i++;
                }
            }
            if (newWord.size() > 1) { // word must have more than one character
                auto newCoords = std::make_pair((char)j, (char)k);
                saveWord(newWord, newCoords, 'V');
            }
            i++;
        }
    }
    //get all words that are horizontal
    for (int k = 0; k < numLines; k++) {
        std::vector<char> Line = getLine(k);
        int i = 0, j = 0;
        while (i < numCols) {
            std::string newWord = "";
            while (Line.at(i) != '0') {
                if (newWord == "") {
                    j = i;
                }
                newWord += Line.at(i);
                if (i == numCols - 1) {
                    break;
                }
                else {
                    i++;
                }
            }
            if (newWord.size() > 1) { // word must have more than one character
                auto newCoords = std::make_pair((char)k, (char)j);
                saveWord(newWord, newCoords, 'H');
            }
            i++;
        }
    }
}

bool Board::getBoardLetters() {
    bool check = true;
    for (int i = 0; i < numLines; i++) {
        std::vector<char> Line = getLine(i);
        for (int j = 0; j < numCols; j++) {
            if (Line.at(j) != '0') {
                totalLetters.push_back(Line.at(j));
            }
        }
    }
    if (totalLetters.size() < 28) {
        error = errors.at(6);
        check = false;
    }
    return check;
}

bool Board::isInDictionary(const std::string &word) {
    std::cout << "size: " << dictWords.size() << '\n';
    return (std::binary_search(dictWords.begin(), dictWords.end(), word));
}

void Board::setPlayed(std::pair<char, char> coords) {
    played.at(coords.first).at(coords.second) = '1';
}

bool Board::isPlayed(std::pair<char, char> coords) {
    return played.at(coords.first).at(coords.second) == '1';
}

std::vector<std::vector<char> > Board::getContent() {
    return board;
}

std::vector<std::pair<char, char> > Board::getWordsInPointStart(std::pair<char, char> coords) {

    std::vector<std::pair<char, char> > result;

    std::cout << "entered function\n";

    // search horizontally
    std::vector<char> currLine = getLine(coords.first);
    std::cout << "got line\n";
    size_t i = coords.second;
    while (currLine.at(i) != '0') {
        std::cout << "in loop\n";
        if (i == 0) {
            break;
        }
        i --;
    }
    std::pair<char, char> startH = {coords.first, i+1};
    std::cout << "pushingback\n";
    if (!getWord(startH, 'H').empty()) result.push_back(startH); else result.emplace_back(-1, -1);


    // search vertically
    std::vector<char> currCol = getCol(coords.second);
    size_t j = coords.first;
    while (currCol.at(j) != '0') {
        std::cout << "inloop\n";
        if (j == 0) break;
        j --;
    }
    std::pair<char, char> startV = {j+1, coords.second};
    std::cout << "pushingback\n";
    if (!getWord(startV, 'V').empty()) result.push_back(startV); else result.emplace_back(-1, -1);

    return result;
}

bool Board::isFinished() {
    for (unsigned i = 0; i < getnLines(); i ++) {
        for (unsigned j = 0; j < getnCols(); j ++) {
            if (!isPlayed({i, j})) {
                return false;
            }
        }
    }
    return true;
}

bool Board::finishedWord(std::pair<char, char> coords) {

    bool finishedH, finishedV;

    // search vertically
    for (size_t i = coords.first; i > getWordsInPointStart(coords).at(1).first; i --) {
        if (!isPlayed({i, coords.second})) finishedV = false;
    }

    // search horizontally
    for (size_t i = coords.second; i > getWordsInPointStart(coords).at(0).second; i --) {
        if (!isPlayed({coords.first, i})) finishedH = false;
    }

    return finishedV || finishedH;

}