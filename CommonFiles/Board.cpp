
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "../CommonFiles/utility.h"
#include "Board.h"

Board::Board() = default;

Board::Board(unsigned rows, unsigned cols)
        : numLines(rows), numCols(cols) {

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

    const size_t W = 1;  // edit to change how close together the _letters are in the board

    // column _letters
    stream << "\n    ";
    for (size_t i = 0; i < numCols; i++) {
        stream << std::left << std::setw(W) << char(i + 97) << " ";
    }

    stream << '\n';
    stream << std::setfill('-') << std::setw(numCols * W + numCols + 4) << std::left << "    " << "\n";

    for (unsigned i = 0; i < numLines; i++) {

        stream << std::setfill(' ') << std::right << std::setw(2) << char(i + 65) << " |" ; // uppercase _letters

        for (size_t j = 0; j < numCols; j++) {
            if (played.at(i).at(j) == '1') {
                board.at(i).at(j) == '0' ? stream << std::left << std::setw(W + 1) << " " : stream << std::left
                << std::setw(W) << _playedColour <<(char) toupper(board.at(i).at(j)) << _noColour << ' ';
            } else {
                board.at(i).at(j) == '0' ? stream << std::left << std::setw(W + 1) << " " : stream << std::left
                << std::setw(W) << (char) toupper(board.at(i).at(j))<< ' ';
            }

        }
        stream << '\n';
    }
}

void Board::insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation) {

    if (orientation == 'V') {
        for (size_t i = coords.first; i < coords.first + word.length(); i++) {
            board.at(i).at(coords.second) = word.at(i - coords.first);
        }
    } else if (orientation == 'H') {
        for (size_t i = coords.second; i < coords.second + word.length(); i++) {
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

void Board::deleteWord(std::pair<char, char> coords, const char& dir, std::vector<char> sequence) {
    
    size_t brdL = coords.first, brdC = coords.second;
    
    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

    if (dir == 'H') {
        for (size_t i = 0; i < numCols; i++) {
            if (sequence.at(i) != line.at(i)) {
                board.at(brdL).at(i) = '0';
            }
        }
    }
    else if (dir == 'V') {
        for (size_t i = 0; i < numLines; i++) {
            if (sequence.at(i) != col.at(i)) {
                board.at(i).at(brdC) = '0';
            }
        }
    }
}

/**
Checks if it is valid to insert the word (entered by the user) in the board
@param word - word entered by the user
@param coords - the coordinates of the word's beginning
@param dir - orientation of the word ('V' or 'H')
@return false if word or word's place are not valid, otherwise true
*/
bool Board::verifyWord(const std::string & word, std::pair<char, char> coords, const char& dir) {

    bool check = true;

    if (!isInDictionary(word)) {
        error = errors.at(3); // word is not in the dictionary
        check = false;
    }
    else if (!isInBoardLimits(word, coords, dir)){
        error = errors.at(0);  // crosses board limits
        check = false;
    }
    else if (badIntersection(word, coords, dir)) {
        error = errors.at(1); // causes bad intersection
        check = false;
    }
    else if ((!checkWordLimits(word, coords, dir)) || (!checkWordNeighborhood(word, coords, dir)) || (!checkNewWords(word, coords, dir))) {
        error = errors.at(2);  // touches existing word
        check = false;
    }
    return check;
}

bool Board::isInBoardLimits(const std::string& word, std::pair<char, char> coords, const char& dir) {
    if (dir == 'H' && (numCols - coords.second) < word.size() || (dir == 'V' && (numLines - coords.first) < word.size())) {
        return false;
    }
    return true;
}

bool Board::badIntersection(const std::string& word, std::pair<char, char> coords, const char& dir) {
    size_t wLen = word.size(), brdL = coords.first, brdC = coords.second;
    size_t hLimit = (wLen + brdC); //position (column) next to the last horizontal word position
    size_t vLimit = (wLen + brdL); //position (line) next to the last vertical word position
    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

    if (dir == 'H') {
        for (size_t i = brdC; i < hLimit; i++) {
            if ((line.at(i) != '0') && (word.at(i - brdC) != line.at(i))) { 
                return true;
            }
        }
    }
    else if (dir == 'V') {
        for (unsigned i = brdL; i < vLimit; i++) {
            if ((col.at(i) != '0') && (word.at(i - brdL) != col.at(i))) {
                return true;
            }
        }
    }
    return false;
}

bool Board::checkWordLimits(const std::string& word, std::pair<char, char> coords, const char& dir) {
    size_t wLen = word.size(), brdL = coords.first, brdC = coords.second;
    size_t hLimit = (wLen + brdC);
    size_t vLimit = (wLen + brdL);
    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

    if (dir == 'H' && (brdC != 0 || (hLimit) != numCols)) {
        if ((brdC == 0 && line.at(hLimit) != '0') || ((hLimit) == numCols && line.at(brdC - 1) != '0')
            || ((brdC != 0 && (hLimit) != numCols) && (line.at(hLimit) != '0' || line.at(brdC - 1) != '0'))) {

            insertWord(word, coords, dir);
            std::vector<char> newLine = getLine(brdL);
            if (!checkNewWord(newLine, dir, 0)) {
                deleteWord(coords, dir, line);
                return false;
            }
            deleteWord(coords, dir, line);
        }
    }
    else if (dir == 'V' && (brdL != 0 || (vLimit) != numLines)) {
        if ((brdL == 0 && col.at(vLimit) != '0') || (vLimit == numLines && col.at(brdL - 1) != '0')
            || ((brdL != 0 && (vLimit) != numLines) && (col.at(vLimit) != '0' || col.at(brdL - 1) != '0'))) {
            
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

bool Board::checkWordNeighborhood(const std::string& word, std::pair<char, char> coords, const char& dir) {
    
    size_t wLen = word.size(), brdL = coords.first, brdC = coords.second;
    bool check = true;

    if (dir == 'H' && ((brdL != 0 || (brdL + 1) != numLines))) {
        
        if ((brdL + 1) != numLines) {
            std::vector<char> nextLine = getLine(brdL + 1);
            check = checkLine(nextLine, coords, word);   //if wordLine != lastLine checks the inferior nighborhood
        }
        if (brdL != 0 && check) {
            std::vector<char> prevLine = getLine(brdL - 1);
            check = checkLine(prevLine, coords, word);  //if wordLine != firstLine just checks the superior nighborhood
        }
    }

    else if (dir == 'V' && (brdC != 0 || ((brdC + 1) != numCols))) {

        if ((brdC + 1) != numCols){
            std::vector<char> nextCol = getCol(brdC + 1);
            check = checkCol(nextCol, coords, word);    //if wordColumn != lastColumn checks the right nighborhood
        }
        if (brdC != 0 && check) {
            std::vector<char> prevCol = getCol(brdC - 1);
            check = checkCol(prevCol, coords, word);    //if wordColumn != firsColumn checks the left nighborhood
        }
    }
    return check;
}

bool Board::checkNewWords(const std::string& word, std::pair<char, char> coords, const char& dir) {
    
    //checks if adding the new word an existing word is updated to a larger word and if this last is valid

    size_t wLen = word.size(), brdL = coords.first, brdC = coords.second;
    size_t hLimit = (wLen + brdC);
    size_t vLimit = (wLen + brdL);
    std::vector<char> line = getLine(brdL);
    std::vector<char> col = getCol(brdC);

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
    }
    else if (dir == 'V') {
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

size_t Board::getnLines() const {
    return numLines;
}

size_t Board::getnCols() const {
    return numCols;
}

std::string Board::getWord(std::pair<char, char> coords, const char& dir) {
    unsigned int brdL = coords.first, brdC = coords.second;
    std::string word{};
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

std::vector<char> Board::getLine(size_t line) {

    return board.at(line);
}

std::vector<char> Board::getCol(size_t col) {

    std::vector<char> column;

    column.reserve(getnLines());

    for (size_t i = 0; i < getnLines(); i++) {
        column.push_back(board.at(i).at(col));
    }

    return column;

}

bool Board::checkLine(std::vector<char> line, std::pair<char, char> coords, const std::string & word) const {
    unsigned int wLen = word.size(), brdC = coords.second;
    size_t hLimit = (wLen + brdC);

    for (size_t i = brdC-1; i <= hLimit; i++) {
        std::string newWord;
        while (i < numCols && i>=0 && i <= hLimit && line.at(i) != '0') {
             newWord += line.at(i);
             if (newWord.size() >= 2) {
                return false;
             }
             i++;
        }
    }
    return true;
}

bool Board::checkCol(std::vector<char> col, std::pair<char, char> coords, const std::string & word) const {
    unsigned int wLen = word.size(), brdL = coords.first;
    size_t vLimit = (wLen + brdL);
 
    for (size_t i = brdL - 1; i <= vLimit; i++){
        std::string newWord;
        while (i >= 0 && i < numLines && i <= vLimit && col.at(i) != '0') {
            newWord += col.at(i);
            if (newWord.size() >= 2) {
                return false;
            }
            i++;
        }
    }
    return true;
}

bool Board::checkNewWord(std::vector<char> sequence, const char& dir, unsigned int flag) {
    
    int num{};
        //Não fiques Noddy com estas flags; basta colocar um comentário a explicar qual a finalidade delas e acho que não há problema;
    if ((dir == 'V' && flag == 1) || (dir == 'H' && flag == 0)) {
        num = numCols;
    }
    else if ((dir == 'H' && flag == 1) || (dir == 'V' && flag == 0)){
        num = numLines;
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

std::vector<std::string> Board::getBoardWords() {
    std::vector<std::string> BoardWords; //is going to contain all the words of the board
    
    //get all words that are vertical
    for (int k = 0; k < numCols; k++) { // iterate through all the columns
        std::vector<char> col = getCol(k);
        int startingLine{};
        for(size_t i=0; i < numLines; i++) { // iterate through the column
            std::string newWord;
            while (col.at(i) != '0') { //while loop to get a word
                if (newWord.empty()) {
                    startingLine = i;
                }
                newWord += col.at(i);
                if (i == numLines - 1) {
                    break;
                }
                i++;
            }
            if (newWord.size() > 1) { // word must have more than one character
                BoardWords.push_back(newWord);
                auto newCoords = std::make_pair((char)startingLine, (char)k);
                saveWord(newWord, newCoords, 'V');
            }
        }
    }

    //get all words that are horizontal
    for (size_t k = 0; k < numLines; k++) { //iterate through all the lines
        std::vector<char> line = getLine(k);
        int startingCol{};
        for(size_t i = 0; i < numCols; i++){ //iterate through the line
            std::string newWord;
            while (line.at(i) != '0') { //while loop to get a word
                if (newWord.empty()) {
                    startingCol = i;
                }
                newWord += line.at(i);
                if (i == numCols - 1) {
                    break;
                }
                i++;
            }
            if (newWord.size() > 1) { // word must have more than one character
                BoardWords.push_back(newWord);
                auto newCoords = std::make_pair((char)k, (char)startingCol);
                saveWord(newWord, newCoords, 'H');
            }
        }
    }
    return BoardWords;
}

std::vector<char> Board::getBoardLetters() {
    for (size_t i = 0; i < numLines; i++) {
        std::vector<char> Line = getLine(i);
        for (size_t j = 0; j < numCols; j++) {
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
    std::vector<char> currCol = getCol((size_t) coords.second);
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
            if (board.at(i).at(j) != '0' && !isPlayed({i, j})) {
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

unsigned Board::maxPlayersAllowed() {

    size_t tiles = getBoardLetters().size();

    if (tiles == 0) return 0;
    else if (tiles < 14) return 1;
    else if (tiles < 21) return 2;
    else if (tiles < 28) return 3;
    else return 4;

}

std::pair<char, char> Board::getFirstNotPlayedH(std::pair<char, char> coords) {

    std::pair<char, char> result{};

    std::pair<char, char> start = getWordsInPointStart(coords).at(0);


    if (start != std::pair<char, char>{-1, -1}) {  // if there is an horizontal  word

        std::string word = getWord(start, 'H');

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


std::pair<char, char> Board::getFirstNotPlayedV(std::pair<char, char> coords) {

    std::pair<char, char> result{};

    std::pair<char, char> start = getWordsInPointStart(coords).at(1);

    if (start != std::pair<char, char>{-1, -1}) {  // if there is a vertical word

        std::string word = getWord(start, 'V');

        for (size_t i = start.first; i < start.first + word.length(); i++) {

            if (!isPlayed({i, coords.second})) {
                result = {i, coords.second};
                break;
            }

        }

    }

    return result;
}
