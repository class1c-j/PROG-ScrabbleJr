
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "../CommonFiles/utility.h"
#include "Board.h"

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
    char sep{};
    s_in1 >> numLines >> sep >> numCols;

    while (std::getline(f_in, line)) {
        if (line.empty()) break;
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
        char x{}, y{}, d{};
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
        if (word.at(3)=='H')
            f_out << word << '\n';
    }
    for (const std::string& word : words) {
        if (word.at(3) == 'V')
            f_out << word << '\n';
    }
    showBoard(f_out);
}

void Board::showBoard(std::ostream& stream) {

    const int W = 1;  // edit to change how close together the letters are in the board

    // column letters
    stream << "\n    ";
    for (int i = 0; i < numCols; i++) {
        stream << std::left << std::setw(W) << char(i + 97) << " ";
    }

    stream << '\n';
    stream << std::setfill('-') << std::setw(numCols * W + numCols + 4) << std::left << "    " << "\n";

    for (unsigned i = 0; i < numLines; i++) {

        stream << std::setfill(' ') << std::right << std::setw(2) << char(i + 65) << " |" ; // uppercase letters

        for (size_t j = 0; j < numCols; j++) {
            if (played.at(i).at(j) == '0') {
                board.at(i).at(j) == '0' ? stream << std::left << std::setw(W + 1) << " " : stream << std::left
                << std::setw(W) << (char) toupper(board.at(i).at(j)) << ' ';
            } else {
                //setBackgroundColor(1);
                stream << "\033[94;103m";
                board.at(i).at(j) == '0' ? stream << std::left << std::setw(W + 1) << " " : stream << std::left
                << std::setw(W) << (char) toupper(board.at(i).at(j))<< ' ';
                setBackgroundColor(0);
            }

        }
        stream << '\n';
    }
}

void Board::insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation) {

    if (orientation == 'V') {
        for (unsigned i = coords.first; i < coords.first + word.length(); i++) {
            board.at(i).at(coords.second) = word.at(i - coords.first);
        }
    } else if (orientation == 'H') {
        for (unsigned i = coords.second; i < coords.second + word.length(); i++) {
            board.at(coords.first).at(i) = word.at(i - coords.second);
        }
    }
}


void Board::removeWord(std::pair<char, char> coords, const char dir) {
    std::string word = getWord(coords, dir);
    unsigned int brdL = coords.first, brdC = coords.second;
    if (!word.empty()) {
        if (dir == 'V') {
            for (size_t i = brdL; i < brdL + word.length(); i++) {
                if ((brdC == 0 || board.at(i).at((brdC)-1) == '0') && ((brdC + 1) == numCols || (board.at(i).at((brdC)+1) == '0'))) {
                    board.at(i).at(brdC) = '0';
                }
            }
        }
        else if (dir == 'H') {
            for (size_t i = brdC; i < brdC + word.length(); i++) {
                if ((brdL == 0 || board.at(brdL - 1).at(i) == '0') && ((brdL + 1) == numLines || (board.at(brdL+1).at(i) == '0'))) {
                    board.at(brdL).at(i) = '0';
                }
            }
        }
    }
    else {
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
            } else if ((brdL + 1) == numLines) {
                std::vector<char> prevLine = getLine(brdL - 1);
                check = checkLine(prevLine, coords, word);
            } else {
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
        for (size_t i = brdC; i < brdC + wLen; i++) {
            std::vector<char> Col = getCol(i);
            if (check) {
                char a = Col.at(i);
                Col.at(brdL) = word.at(i - brdC);
                int j = 0;
                while (j < numLines) {
                    std::string newWord;
                    while (Col.at(j) != '0') {
                        newWord += Col.at(j);
                        if (j == numLines - 1) {
                            break;
                        }
                        j++;
                    }
                    if (newWord.size() > 1) { // word must have more than one character
                        if (!(isInDictionary(newWord))) {
                            check = false;
                            break;
                        }
                    }
                    j++;
                }
                Col.at(brdC) = a;
                if (!check) {
                    error = errors.at(2);
                }
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
        if ((brdC != 0 || ((brdC + 1) != numCols)) && check) {
            if (brdC == 0) {
                std::vector<char> nextCol = getCol(1);
                check = checkCol(nextCol, coords, word);
            } else if ((brdC + 1) == numCols) {
                std::vector<char> prevCol = getCol(brdC - 1);
                check = checkCol(prevCol, coords, word);
            } else {
                std::vector<char> nextCol = getCol(brdC + 1);
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
        for (size_t i = brdL; i < brdL + wLen; i++) {
            std::vector<char> Line = getLine(i);
            if (check) {
                char a = Line.at(i);
                Line.at(brdC) = word.at(i - brdL);
                int j = 0;
                while (j < numCols) {
                    std::string newWord;
                    while (line.at(j) != '0') {
                        newWord += line.at(j);
                        if (j == numCols - 1) {
                            break;
                        }
                        j++;
                    }
                    if (newWord.size() > 1) { // word must have more than one character
                        if (!(isInDictionary(newWord))) {
                            check = false;
                            break;
                        }
                    }
                    j++;
                }
                Line.at(brdC) = a;
                if (!check) {
                    error = errors.at(2);
                }
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

std::string Board::getWord(std::pair<char, char> coords, const char& dir) {
    unsigned int brdL = coords.first, brdC = coords.second;
    std::string word;
    if (dir == 'V') {
        std::vector<char> Col = getCol(brdC);
        size_t i = brdL;
        while (i < numLines && Col.at(i) != '0') {
            if (isalpha(Col.at(i))) word += Col.at(i);
            i++;
        }
    }
    else if (dir == 'H') {
        std::vector<char> Line = getLine(brdL);
        size_t i = brdC;
        while (i < numCols && Line.at(i) != '0') {
            if (isalpha(Line.at(i))) word += Line.at(i);
            i++;
        }
    }
    return word;
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

std::string Board::searchWord(std::pair<char, char> coords, const char &dir) {
    std::string pos = std::string() + (char) (coords.first + 65) + (char) (coords.second + 97);
    for (const auto &i : words) {
        if (i.substr(0, 4) == pos + ' ' + dir) {
            return i.substr(5);
        }
    }
    return std::string();
}

std::vector<char> Board::getLine(unsigned int line) {
    return board.at(line);
}

std::vector<char> Board::getCol(unsigned int col) {
    std::vector<char> column;
    column.reserve(getnLines());
    for (int i = 0; i < getnLines(); i++) {
        column.push_back(board.at(i).at(col));
    }
    return column;
}


bool Board::checkLine(std::vector<char> line, std::pair<char, char> coords, const std::string &word) const {
    unsigned int wLen = word.size(), brdC = coords.second;
    int i = brdC;
    while (i <= (brdC + wLen)) {
        std::string newWord;
        if (i >= 0 && i < numCols) {
            while (line.at(i) != '0' && i <= (brdC + wLen)) {
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

bool Board::checkCol(std::vector<char> col, std::pair<char, char> coords, const std::string &word) const {
    unsigned int wLen = word.size(), brdL = coords.first;
    unsigned i = brdL - 1;
    while (i <= (brdL + wLen)) {
        std::string newWord;
        if (i >= 0 && i < numLines) {
            while (col.at(i) != '0' && i <= (brdL + wLen)) {
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


std::vector<std::string> Board::getBoardWords() {
    std::vector<std::string> BoardWords;
    //get all words that are vertical
    for (int k = 0; k < numCols; k++) {
        std::vector<char> Col = getCol(k);
        int i = 0, j = 0;
        while (i < numLines) {
            std::string newWord;
            while (Col.at(i) != '0') {
                if (newWord.empty()) {
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
                BoardWords.push_back(newWord);
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
            std::string newWord;
            while (Line.at(i) != '0') {
                if (newWord.empty()) {
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
                BoardWords.push_back(newWord);
                auto newCoords = std::make_pair((char)k, (char)j);
                saveWord(newWord, newCoords, 'H');
            }
            i++;
        }
    }
    return BoardWords;
}

std::vector<char> Board::getBoardLetters() {
    for (unsigned i = 0; i < numLines; i++) {
        std::vector<char> Line = getLine(i);
        for (int j = 0; j < numCols; j++) {
            if (Line.at(j) != '0') {
                totalLetters.push_back(Line.at(j));
            }
        }
    }
    if (totalLetters.size() < 28) {
        error = errors.at(6);
    }
    return totalLetters;
}

bool Board::isInDictionary(const std::string &word) {
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


    // search horizontally
    std::vector<char> currLine = getLine(coords.first);
    std::pair<char, char> startH{};
    size_t i = coords.second;

    if (i == 0) {
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
    std::vector<char> currCol = getCol((int) coords.second);
    std::pair<char, char> startV{};
    size_t j = coords.first;

    if (j == 0) {
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

bool Board::isFinished() {
    for (unsigned i = 0; i < getnLines(); i++) {
        for (unsigned j = 0; j < getnCols(); j++) {
            if (!isPlayed({i, j})) {
                return false;
            }
        }
    }
    return true;
}

int Board::finishedWord(std::pair<char, char> coords) {

    bool finishedH = true, finishedV = true;

    // search vertically
    std::pair<char, char> startV = getWordsInPointStart(coords).at(1);
    size_t sizeV = searchWord(startV, 'V').size();
    if (sizeV == 0) finishedV = false;
    size_t endV = startV.first + sizeV;
    for (size_t i = startV.first; i < endV; i ++) {
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
    for (size_t i = startH.second; i < endH; i ++) {
        if (!isPlayed({coords.first, i})) {
            finishedH = false;
            break;
        }
    }

    return int(finishedV) + int(finishedH);

}
