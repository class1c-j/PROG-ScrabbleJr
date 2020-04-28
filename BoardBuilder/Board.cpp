#include "Board.h"
#include <iostream>
#include <iomanip>

Board::Board() = default;

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
    getline(f_in, line);
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

void Board::save(const std::string &name) {
    std::ifstream test(name);
    std::ofstream f_out(name);
    f_out << numLines << " x " << numCols << '\n';
    for (const std::string &word : words) {
        f_out << word << '\n';
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


unsigned Board::getnLines() const {
    return numLines;
}

unsigned Board::getnCols() const {
    return numCols;
}


// coords -> {linha, coluna} -> {maiuscula, minuscula}
void Board::insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation) {

    if (orientation == 'V') {  // a coluna é sempre a mesma, apenas iterar a linha
        for (int i = coords.first; i < coords.first + word.size(); i++) {
            board.at(i).at(coords.second) = word.at(i - coords.first);
        }
    } else if (orientation == 'H') {  // linha é sempre a mesma, apenas iterar a coluna
        for (int i = coords.second; i < coords.second + word.size(); i++) {
            board.at(coords.first).at(i) = word.at(i - coords.second);
        }
    }

    saveWord(word, coords, orientation);
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

void Board::saveWord(const std::string &word, std::pair<char, char> coords, const char &dir) {
    std::string pos = std::string() + (char) (coords.first + 65) + (char) (coords.second + 97);
    std::string info = pos + ' ' + dir + ' ' + word;
    words.push_back(info);
}


bool Board::verifyWord(const std::string &word, std::pair<char, char> coords, const char &dir) {

    bool check = true;
    unsigned wLen = word.size(), brdL = coords.first, brdC = coords.second, counter = 0;

    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

    std::string lStr(line.begin(), line.end());
    std::string cStr(col.begin(), col.end());

    if ((dir == 'H' && (numCols - brdC) < wLen) || (dir == 'V' && (numLines - brdL) < wLen)) {

        check = false;
        std::cout << "ERROR: Out of board limits\n";

    } else if (dir == 'H') {

        for (unsigned i = brdC; i < (wLen + brdC); i++) {
            if (line.at(i) != '0') {
                counter++; //counter to check if the word is not inside a word that was already inserted
                if (word.at(i - brdC) != line.at(i)) {
                    std::cout << "ERROR: Bad intersection\n";
                    check = false;
                    break;
                }
            }
        }
        if (brdC != 0 || (brdC + wLen) != numCols) {

            //condição para verificar as 2 extremidades horizontais se uma delas estiver nos limites do horizontais do tabuleiro
            if ((brdC == 0 && line.at(brdC + wLen) != '0') || ((brdC + wLen) == numCols && line.at(brdC - 1) != '0')) {
                std::cout << "ERROR: Word already next to this word\n";
                check = false;
            }
                //condição para verificar as 2 extremidades horizontais se nenhuma delas estiver nos limites horizontais do tabuleiro
            else if (brdC != 0 && (brdC + wLen) != numCols && line.at(brdC + wLen) != '0' && line.at(brdC - 1) != '0') {
                std::cout << "ERROR: Word already next to this word\n";
                check = false;
            }
        }
    } else if (dir == 'V') {
        for (unsigned i = brdL; i < (wLen + brdL); i++) {
            if (col.at(i) != '0') {
                counter++; //counter to check if the word is not inside a word that was already inserted
                if (word.at(i - brdL) != col.at(i)) {
                    std::cout << "ERROR: Bad intersection\n";
                    check = false;
                    break;
                }
            }
        }
        if (brdL != 0 || (brdL + wLen) != numLines) {

            //condição para verificar as 2 extremidades verticais se uma delas estiver nos limites do vertcias do tabuleiro
            if ((brdL == 0 && col.at(brdL + wLen) != '0') || ((brdL + wLen) == numLines && col.at(brdL - 1) != '0')) {
                std::cout << "ERROR: Word already next to this word\n";
                check = false;
            }
                //condição para verificar as 2 extremidades verticais se nenhuma delas estiver nos limites verticais do tabuleiro
            else if (brdL != 0 && (brdL + wLen) != numLines && col.at(brdL + wLen) != '0' && col.at(brdL - 1) != '0') {
                std::cout << "ERROR: Word already next to this word\n";
                check = false;
            }
        }
    }

    if (check && counter == wLen) {
        check = false;
        std::cout << "ERROR: Word already in that location\n";
    }
    return check;
}
