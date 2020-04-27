//
// Created by ritam on 25/04/20.
//

#include "Board.h"
#include <iostream>
#include <iomanip>

Board::Board(unsigned rows, unsigned cols) {
    numLines = rows;
    numCols = cols;

    // resize the board vector according to the specified dimensions
    board.resize(numLines);
    for (auto &i : board) {
        i.resize(numCols);
    }

    // initialize it with '0' in every position to mark that there are no pieces placed
    for (size_t i = 0; i < numLines; i++) {
        for (size_t j = 0; j < numCols; j++) {
            board.at(i).at(j) = '0';
        }
    }
}

Board::Board(std::ifstream &f_in) {

    std::string line;
    std::getline(f_in, line);
    std::istringstream s_in1(line);
    char sep;
    s_in1 >> numLines >> sep >> numCols;
    while (std::getline(f_in, line)) {
        words.push_back(line);
    }

    board.resize(numLines);
    for (auto &i : board) {
        i.resize(numCols);
    }

    // initialize it with '0' in every position to mark that there are no pieces placed
    for (size_t i = 0; i < numLines; i++) {
        for (size_t j = 0; j < numCols; j++) {
            board.at(i).at(j) = '0';
        }
    }

    std::cout << words.size();
    for (auto &i : words) {
        std::stringstream str_stream(i);
        char x, y, d;
        std::string word;
        str_stream >> x >> y >> d >> word;
        insertWord(word, {x - 65, y - 97}, d);
    }
}

void Board::saveBoard(const std::string &name) {
    std::ifstream test(name);
    if (!test) {
        std::ofstream f_out(name);
        f_out << numLines << " x " << numCols << '\n';
        for (const std::string &word : words) {
            f_out << word << '\n';
        }
    } else {
        std::cout << "File already exists!!1";
    }
}

void Board::showBoard() {

    const int W = 2;  // edit to change how close together the letters are in the board

    // lowercase letters
    std::cout << "\n    ";
    for (int i = 0; i < numCols; i++) {
        std::cout << std::left << std::setw(W) << char(i + 97) << " ";
    }

    std::cout << '\n';
    std::cout << std::setfill('_') << std::setw(numCols * W + numCols + W) << std::left << "    " << "\n";

    for (int i = 0; i < numLines; i++) {

        std::cout << std::setfill(' ') << std::right << std::setw(2) << char(i + 65) << " |";   // uppercase letters

        for (int j = 0; j < numCols; j++) {
            if (board.at(i).at(j) == '0') {  // do not show the 0 in empty positions
                std::cout << std::left << std::setw(W + 1) << " ";
            } else {
                std::cout << std::left << std::setw(W) << board.at(i).at(j) << ' ';
            }
        }

        std::cout << '\n';
    }
}


// coords -> {linha, coluna} -> {maiuscula, minuscula}
<<<<<<< HEAD
void Board::insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation) {

    //std::string pos = std::string() + (char) (coords.first + 65) + (char) (coords.second + 97);
    //std::string info = pos + ' ' + orientation + ' ' + word;
    //words.push_back(info);
    // TODO: put this in a separate method or something. This should be done after manually inserting a word

    if (orientation == 'V') {  // a coluna é sempre a mesma, apenas iterar a linha
        for (int i = coords.first; i < coords.first + word.size(); i++) {
            board.at(i).at(coords.second) = word.at(i - coords.first);
        }
    } else if (orientation == 'H') {  // linha é sempre a mesma, apenas iterar a coluna
        for (int i = coords.second; i < coords.second + word.size(); i++) {
            board.at(coords.first).at(i) = word.at(i - coords.second);
=======
void Board::insertWord(const std::string& word, std::pair<char, char> coords, const char& orientation) {
    bool available = verifyBoard(word, coords, orientation);
    if (available) {
        if (orientation == 'V') {  // a coluna é sempre a mesma, apenas iterar a linha
            for (int i = coords.first; i < coords.first + word.size(); i++) {
                board.at(i).at(coords.second) = word.at(i - coords.first);
            }
        }
        else if (orientation == 'H') {  // linha é sempre a mesma, apenas iterar a coluna
            for (int i = coords.second; i < coords.second + word.size(); i++) {
                board.at(coords.first).at(i) = word.at(i - coords.second);
            }
>>>>>>> 2f1f47abd84743f9cb35fcc1bc7c84536c8bfa5c
        }
    }
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

<<<<<<< HEAD

bool Board::verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir) {

    bool check = true;
    unsigned wLen = word.size(), brdL = coords.first, brdC = coords.second;

    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

    std::string lStr(line.begin(), line.end());
    std::string cStr(col.begin(), col.end());

    if ((dir == 'H' && (numCols - brdC) < wLen) || (dir == 'V' && (numLines - brdL) < wLen)) {

        check = false;
        std::cout << "ERROR: Out of board limits\n";

    } else if (dir == 'H') {

        for (unsigned i = brdC; i <= wLen; i++) {
            if (line.at(i) != '0' && word.at(i - brdC) != line.at(i)) {
                std::cout << "ERROR: Bad intersection\n";
                check = false;
                break;
            }
        }

    } else if (dir == 'V') {

        for (unsigned i = brdL; i <= wLen; i++) {
            if (col.at(i) != '0' && word.at(i - brdL) != col.at(i)) {
                std::cout << "ERROR: Bad intersection\n";
                check = false;
                break;
            }
        }
    }

    return check;
=======
bool Board::verifyBoard(const std::string& word, std::pair<char, char> coords, const char& orientation) {
    if (orientation == 'H') {
        if ((numCols - coords.second) < word.size()) {
            std::cout << "The inserted word does not respect the dimensions of the board!" << std::endl;
            return false;
        }
        else {
            std::vector<char> line = getLine(coords.first);
            for (int i = coords.second; i <= word.size(); i++) {
                if (line.at(i) != '0') {
                    if (word.at(i - coords.second) != line.at(i)) {
                        std::cout << "The inserted word does not respect the words alrealdy inserted in the board!" << std::endl;
                        return false;
                    }
                }
            }
        }
    }
    else {
        if ((numLines - coords.first) < word.size()) {
            std::cout << "The inserted word does not respect the dimensions of the board!" << std::endl;
            return false;
        }
        else {
            std::vector<char> col = getCol(coords.second);
            for (int i = coords.first; i <= word.size(); i++) {
                if (col.at(i) != '0') {
                    if (word.at(i - coords.first) != col.at(i)) {
                        std::cout << "The inserted word does not respect the words alrealdy inserted in the board!" << std::endl;
                        return false;
                    }
                }
            }
        }
    }
    return true;
>>>>>>> 2f1f47abd84743f9cb35fcc1bc7c84536c8bfa5c
}
