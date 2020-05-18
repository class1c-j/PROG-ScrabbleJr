

#include "Board.h"

Board::Board() = default;

Board::Board(unsigned rows, unsigned cols)
        : m_numLines(rows), m_numCols(cols) {

    // resize the board vector according to the specified dimensions
    m_board.resize(m_numLines);
    m_played.resize(m_numLines);

    for (auto &i : m_board) {
        i.resize(m_numCols);
    }

    // initialize it with '0' in every position to mark that there are no pieces placed
    for (size_t i = 0; i < m_numLines; i++) {
        for (size_t j = 0; j < m_numCols; j++) {
            m_board.at(i).at(j) = '0';
        }
    }

    m_played = m_board;

}

Board::Board(std::ifstream &f_in) {

    // read dimensions
    std::string line;
    getline(f_in, line);
    std::istringstream s_in1(line);
    char sep{};
    s_in1 >> m_numLines >> sep >> m_numCols;

    // create a board with the chosen dimensions
    while (std::getline(f_in, line)) {
        if (line.empty()) break;
        m_wordsInfo.insert(line);
    }

    m_board.resize(m_numLines);
    for (auto &i : m_board) {
        i.resize(m_numCols);
    }

    // initialize the played board
    m_played = m_board;


    // initialize it with '0' in every position to mark that there are no words placed
    for (size_t i = 0; i < m_numLines; i++) {
        for (size_t j = 0; j < m_numCols; j++) {

            m_played.at(i).at(j) = '0';
            m_board.at(i).at(j) = '0';
        }
    }

    // read words
    for (auto &i : m_wordsInfo) {

        std::stringstream str_stream(i);
        char x{}, y{}, d{};
        std::string word;

        str_stream >> x >> y >> d >> word;
        insertWord(word, {x - 65, y - 97}, d);

    }
}

//======================================================================================================================
/**
 * @brief get the number of lines the board has
 * @return number of lines
 */
size_t Board::getNumberLines() const {

    return m_numLines;
}

//======================================================================================================================
/**
 * @brief gets the number of columns the board has
 * @return the number of columns
 */
size_t Board::getNumberCols() const {

    return m_numCols;
}

//======================================================================================================================
/**
 * @brief get the letter in a given position
 * @return the letter
 */
char Board::getLetter(std::pair<char, char> coords) {

    auto line = static_cast<size_t> (coords.first), column = static_cast<size_t> (coords.second);
    return m_board.at(line).at(column);
}

//======================================================================================================================
/**
 * @brief checks the board for any uncovered tile
 * @return true if all the places have been played, false otherwise
 */
bool Board::isFinished() {

    bool finished = true;

    for (unsigned i = 0; i < getNumberLines(); i++) {

        for (unsigned j = 0; j < getNumberCols(); j++) {

            if (m_board.at(i).at(j) != '0' && !isPlayed({i, j})) {
                return false;
            }
        }
    }

    return finished;
}

//======================================================================================================================
/**
 * @brief checks for finished words horizontally and vertically
 * @param coords the coords of the last placed piece
 * @return number of finished words
 */
int Board::getNumberOfCompletedWordsByPlay(std::pair<char, char> coords) {

    bool finishedH = true, finishedV = true;

    // search vertically
    std::pair<char, char> startV = getStartOfWordsInPoint(coords).at(1);
    size_t sizeV = searchWord(startV, 'V').size();

    if (sizeV == 0) finishedV = false;
    auto startVLine = static_cast<size_t>(startV.first);
    size_t endV = static_cast<size_t>(startV.first) + sizeV;

    for (size_t i = startVLine; i < endV; i++) {

        if (!isPlayed({i, coords.second})) {
            finishedV = false;
            break;
        }

    }


    // search horizontally
    std::pair<char, char> startH = getStartOfWordsInPoint(coords).at(0);
    size_t sizeH = searchWord(startH, 'H').size();

    if (sizeH == 0) finishedH = false;
    auto startHColumn = static_cast<size_t>(startH.second);
    size_t endH = static_cast<size_t>(startH.second) + sizeH;

    for (size_t i = startHColumn; i < endH; i++) {

        if (!isPlayed({coords.first, i})) {
            finishedH = false;
            break;
        }

    }

    return int(finishedV) + int(finishedH);

}

//======================================================================================================================
/**
 * @brief check if a place has been played
 * @param coords - the place to check
 * @return true if the place has been played, false otherwise
 */
bool Board::isPlayed(std::pair<char, char> coords) {

    auto line = static_cast<size_t> (coords.first), column = static_cast<size_t> (coords.second);
    return m_played.at(line).at(column) == '1';
}

//======================================================================================================================
/**
 * @brief gets the position of the first not played letter of an horizontal word
 * @param coords - the starting coordinates of the word
 * @return the coordinates of the first free tile to play
 */
std::pair<char, char> Board::getFirstFreeHorizontal(std::pair<char, char> coords) {

    std::pair<char, char> result{};

    std::pair<char, char> start = getStartOfWordsInPoint(coords).at(0);


    if (start != std::pair<char, char>{-1, -1}) {  // if there is an horizontal  word

        std::string word = getWordToRemove(start, 'H');

        auto startColumn = static_cast<size_t> (start.second);

        // iterate through the board positions where the word is, looking for the first free spot
        for (size_t i = startColumn; i < startColumn + word.length(); i++) {

            if (!isPlayed({coords.first, i})) {
                result = {coords.first, i};
                break;
            }

        }

    }

    return result;
}

//======================================================================================================================
/**
 * @brief gets the position of the first not played letter of a vertical word
 * @param coords - the starting coordinates of the word
 * @return the coordinates of the first free tile to play
 */
std::pair<char, char> Board::getFirstFreeVertical(std::pair<char, char> coords) {

    std::pair<char, char> result{};

    std::pair<char, char> start = getStartOfWordsInPoint(coords).at(1);

    if (start != std::pair<char, char>{-1, -1}) {  // if there is a vertical word

        std::string word = getWordToRemove(start, 'V');
        auto startLine = static_cast<size_t> (start.first);

        for (size_t i = startLine; i < startLine + word.length(); i++) {

            if (!isPlayed({i, coords.second})) {
                result = {i, coords.second};
                break;
            }

        }

    }

    return result;
}

//======================================================================================================================
/**
 * @brief looks for the starts of the words that pass in a given point
 * @param coords - the coordinates of the given point
 * @return a vector with the coordinates of the start of the words that pass in the point
 */
std::vector<std::pair<char, char> > Board::getStartOfWordsInPoint(std::pair<char, char> coords) {

    std::vector<std::pair<char, char> > result;
    auto line = static_cast<size_t>(coords.first);

    // search horizontally
    std::vector<char> lineToSearch = getLine(line);
    std::pair<char, char> startH{};
    auto currentColumn = static_cast<size_t> (coords.second);

    if (currentColumn == 0) {  // if the word starts ate the start of the line there's no need to search

        startH = {coords.first, currentColumn};

    } else {

        while (lineToSearch.at(currentColumn) != '0') {
            currentColumn--;
            if (currentColumn == 0) break;
        }

        if (lineToSearch.at(currentColumn) == '0') currentColumn++;
        startH = {coords.first, currentColumn};

    }


    if (!searchWord(startH, 'H').empty()) result.push_back(startH); else result.emplace_back(-1, -1);


    // search vertically
    std::vector<char> columnToSearch = getCol((size_t) coords.second);
    std::pair<char, char> startV{};
    auto currentLine = static_cast<size_t>(coords.first);

    if (currentLine == 0) { // if the word starts ate the start of the column there's no need to search

        startV = {currentLine, coords.second};

    } else {

        while (columnToSearch.at(currentLine) != '0') {
            currentLine--;
            if (currentLine == 0) break;
        }

        if (columnToSearch.at(currentLine) == '0') currentLine++;
        startV = {currentLine, coords.second};
    }

    if (!searchWord(startV, 'V').empty()) result.push_back(startV); else result.emplace_back(-1, -1);

    return result;
}

//======================================================================================================================
/**
 * @brief gets all the letters in the board
 * @return a vector with every letter in the board
 */
std::vector<char> Board::getAllLetters() {

    for (size_t currentLine = 0; currentLine < m_numLines; currentLine++) {

        std::vector<char> line = getLine(currentLine);
        for (size_t currentCol = 0; currentCol < m_numCols; currentCol++) {

            if (line.at(currentCol) != '0') {
                m_allLetters.push_back(line.at(currentCol));
            }
        }
    }

    if (m_allLetters.size() < 28) _error = m_errors.at(6);    // not possible to play the game with 4 players
    return m_allLetters;

}

//======================================================================================================================
/**
 * @brief place a tile
 * @param coords the coordinates of the place where the tile was placed
 */
void Board::setPlayed(std::pair<char, char> coords) {

    auto line = static_cast<size_t> (coords.first), column = static_cast<size_t> (coords.second);
    m_played.at(line).at(column) = '1';
}

//======================================================================================================================
/**
 * @brief inserts a new word in the board
 * @param word - the word to insert
 * @param coords - the starting coordinates
 * @param orientation - the desired direction (vertical or horizontal)
 */
void Board::insertWord(const std::string &word, std::pair<char, char> coords, const char &orientation) {

    auto startingLine = static_cast<size_t>(coords.first);
    auto startingColumn = static_cast<size_t>(coords.second);

    if (orientation == 'V') {

        size_t endingLine = startingLine + word.size();

        for (size_t i = startingLine; i < endingLine; i++) {
            m_board.at(i).at(startingColumn) = word.at(i - startingLine);
        }

    } else if (orientation == 'H') {

        size_t endingColumn = startingColumn + word.size();

        for (size_t i = startingColumn; i < endingColumn; i++) {
            m_board.at(startingLine).at(i) = word.at(i - startingColumn);
        }

    }
}

//======================================================================================================================
/**
 * @brief removes a word from the board
 * @param coords - the starting position of the word
 * @param dir - the orientation of the word
 */
void Board::removeWord(std::pair<char, char> coords, const char dir) {

    std::string word = getWordToRemove(coords, dir);
    auto startLine = static_cast<size_t> (coords.first), startCol = static_cast<size_t> (coords.second);

    if (!word.empty()) {

        if (dir == 'V') {
            size_t endLine = startLine + word.size();

            for (size_t i = startLine; i < endLine; i++) {

                if ((startCol == 0 || m_board.at(i).at((startCol) - 1) == '0') &&
                    ((startCol + 1) == m_numCols || (m_board.at(i).at((startCol) + 1) == '0'))) {

                    m_board.at(i).at(startCol) = '0';  // delete the letter
                }

            }

        } else if (dir == 'H') {
            size_t endColumn = startCol + word.size();

            for (size_t i = startCol; i < endColumn; i++) {

                if ((startLine == 0 || m_board.at(startLine - 1).at(i) == '0') &&
                    ((startLine + 1) == m_numLines || (m_board.at(startLine + 1).at(i) == '0'))) {

                    m_board.at(startLine).at(i) = '0';  // delete the letter
                }
            }

        }

    } else {
        _error = m_errors.at(4);  // no word to remove
    }
}

//======================================================================================================================
/**
 * @brief load all the words from the provided dictionary into a vector
 */
void Board::readDictionary() {

    std::string word;
    std::ifstream dict("../BoardBuilder/WORDS.TXT");

    if (dict.is_open()) {

        while (dict >> word) {
            m_dictionary.push_back(word);
        }

        dict.close();

    } else {
        _error = m_errors.at(5);
    }
}

//======================================================================================================================
/**
 * @brief save the board in a file
 * @param name - the name of the file
 */
void Board::save(const std::string &name) {

    saveAllWords();  // get every word present in the board

    std::ofstream f_out(name);  // create a file

    f_out << m_numLines << " x " << m_numCols << '\n';  // write dimensions

    for (const std::string &word : m_wordsInfo) {  // write horizontal words info
        if (word.at(3) == 'H')
            f_out << word << '\n';
    }

    for (const std::string &word : m_wordsInfo) {  // write vertical words info
        if (word.at(3) == 'V')
            f_out << word << '\n';
    }

    show(f_out);  // write board preview
}

//================================================================================
/**
 * @brief pass the board to a ostream allowing it to "show" it in the terminal or in a file
 * @param stream - the stream used
 */
void Board::show(std::ostream &stream) {

    const int W = 1;  // edit to change how close together the _letters are in the board

    // show column indicating letters
    stream << "\n    ";

    for (size_t i = 0; i < m_numCols; i++) {
        stream << std::left << std::setw(W) << char(i + 97) << " ";
    }

    auto columns = static_cast<int> (m_numCols);
    int width = columns * W + columns + 4;   // casting outside of the setw to silence a vs compiler warning

    stream << '\n';
    // show line separating the column indicator letters from the board itself
    stream << std::setfill('-') << std::setw(width) << std::left << "    " << "\n";

    for (unsigned i = 0; i < m_numLines; i++) {

        // show the line indicating letter
        stream << std::setfill(' ') << std::right << std::setw(2) << char(i + 65) << " |";

        // show the board line, with attention to whether or not it a tile has been played
        for (size_t j = 0; j < m_numCols; j++) {
            if (m_played.at(i).at(j) == '1') {
                m_board.at(i).at(j) == '0' ? stream << std::left << std::setw(W + 1) << " " : stream << std::left
                << std::setw(W) << _ENGINEERING << (char) toupper(m_board.at(i).at(j)) << _DEFAULT << ' ';
            } else {
                m_board.at(i).at(j) == '0' ? stream << std::left << std::setw(W + 1) << " " : stream << std::left
                << std::setw(W) << (char) toupper(m_board.at(i).at(j)) << ' ';
            }

        }

        stream << "\n";
    }
}

//======================================================================================================================
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

        _error = m_errors.at(3); // word is not in the dictionary
        check = false;

    } else if (!isInBoardLimits(word, coords, dir)) {

        _error = m_errors.at(0);  // crosses board limits
        check = false;

    } else if (badIntersection(word, coords, dir)) {

        _error = m_errors.at(1); // causes bad intersection
        check = false;

    } else if ((!validWordLimits(word, coords, dir)) || (!validWordNeighborhood(word, coords, dir)) ||
               (!causesValidWordAdditions(word, coords, dir))) {

        _error = m_errors.at(2);  // touches existing word
        check = false;

    }

    return check;
}

//======================================================================================================================
/**
 * @brief gets the selected line in a vector
 * @param line - the number of the line
 * @return a vector containing all the elements of the line
 */
std::vector<char> Board::getLine(size_t line) {

    return m_board.at(line);
}

//======================================================================================================================
/**
 * @brief gets the selected column in a vector
 * @param column - the number of the column
 * @return a vector containing all the elements of the column
 */
std::vector<char> Board::getCol(size_t column) {

    std::vector<char> columnElements;

    columnElements.reserve(getNumberLines());

    for (size_t i = 0; i < getNumberLines(); i++) {
        columnElements.push_back(m_board.at(i).at(column));
    }

    return columnElements;

}

//======================================================================================================================
/**
 * @brief searches the board for all the letters of the word selected to remove
 * @param coords the starting point of the word
 * @param dir the direction of the word
 * @return a string with the word
 */
std::string Board::getWordToRemove(std::pair<char, char> coords, const char &dir) {

    auto startLine = static_cast<size_t> (coords.first), startCol = static_cast<size_t> (coords.second);
    std::string word{};

    if (dir == 'V') {

        std::vector<char> column = getCol(startCol);
        size_t currentLine = startLine;

        while (currentLine < m_numLines && column.at(currentLine) != '0') {

            if (isalpha(column.at(currentLine))) word += column.at(currentLine);
            currentLine++;
        }

    } else if (dir == 'H') {

        std::vector<char> line = getLine(startLine);
        size_t currentCol = startCol;

        while (currentCol < m_numCols && line.at(currentCol) != '0') {

            if (isalpha(line.at(currentCol))) word += line.at(currentCol);
            currentCol++;
        }

    }

    return word;
}

//======================================================================================================================
/**
 * @brief searches the board for a word
 * @param coords - the starting point of the word
 * @param direction - the direction of the word
 * @return if there is a word matching the criteria, it is returned, otherwise returns an empty string
 */
std::string Board::searchWord(std::pair<char, char> coords, const char &direction) {

    std::string position = std::string() + (char) (coords.first + 65) + (char) (coords.second + 97);

    for (const auto &i : m_wordsInfo) {

        if (i.substr(0, 4) == position + ' ' + direction) {

            return i.substr(5);
        }

    }

    return std::string();
}

//======================================================================================================================
/**
 * @brief scans the board and saves all of it's words
 */
void Board::saveAllWords() {

    //get all words that are vertical
    for (size_t currentCol = 0; currentCol < m_numCols; currentCol++) {

        std::vector<char> column = getCol(currentCol);
        size_t startingLine{};

        for (size_t currentLine = 0; currentLine < m_numLines; currentLine++) {

            std::string foundWord{};  // temporary string to hold the letters found in the current column

            while (column.at(currentLine) != '0') { // found a word, gather all of it's letters

                if (foundWord.empty()) startingLine = currentLine;

                foundWord += column.at(currentLine);

                if (currentLine == m_numLines - 1) break;  // reached the end of the column

                currentLine++;
            }

            if (foundWord.size() > 1) { // found a valid word

                auto coords = std::make_pair((char) startingLine, (char) currentCol);
                saveWord(foundWord, coords, 'V');

            }
        }
    }



    //get all words that are horizontal
    for (size_t currLine = 0; currLine < m_numLines; currLine++) {

        std::vector<char> line = getLine(currLine);
        size_t startingColumn{};

        for (size_t currCol = 0; currCol < m_numCols; currCol++) {

            std::string foundWord;  // temporary string to hold the letters found in the current column

            while (line.at(currCol) != '0') {  // found a word, gather all of it's letters

                if (foundWord.empty()) startingColumn = currCol;

                foundWord += line.at(currCol);

                if (currCol == m_numCols - 1) break;   // reached the end of the column

                currCol++;
            }

            if (foundWord.size() > 1) { // found a valid word

                auto newCoords = std::make_pair((char) currLine, (char) startingColumn);
                saveWord(foundWord, newCoords, 'H');
            }

        }
    }
}

//======================================================================================================================
/**
 * @brief saves a word in the format "Lc D word" into a board's vector to later be able to generate the file
 * @param word - the word
 * @param coords - the starting point of the word
 * @param dir - the direction
 */
void Board::saveWord(const std::string &word, std::pair<char, char> coords, const char &dir) {

    std::string pos = std::string() + (char) (coords.first + 65) + (char) (coords.second + 97);
    std::string info = pos + ' ' + dir + ' ' + word;

    m_startingPoints.insert(pos);
    m_wordsInfo.insert(info);

}

//======================================================================================================================
/**
 * @brief checks if the insertion of a word causes a valid horizontal neighborhood
 * @param line - the line to check
 * @param coords - word starting positions
 * @param word - word entered by the user
 * @return true if the horizontal neighborhood is valid, false otherwise
 */
bool Board::isValidHorizontalNeighborhood(std::vector<char> line, std::pair<char, char> coords,
                                          const std::string &word) const {

    auto wordLength = static_cast<int> (word.size()), startColumn = static_cast<int> (coords.second);
    int horizontalLimit = (wordLength + startColumn);
    bool validNeighborhood = true;

    for (int currColumn = startColumn - 1; currColumn <= horizontalLimit; currColumn++) {

        std::string lettersInNeighborhood{};  // temporary string to hold every letter in the neighborhood

        while (currColumn < static_cast<int>(m_numCols) && currColumn >= 0 && currColumn <= horizontalLimit
               && line.at(static_cast<size_t>(currColumn)) != '0') {

            // found a letter in the neighborhood

            lettersInNeighborhood += line.at(static_cast<size_t> (currColumn));

            if (lettersInNeighborhood.size() >= 2) return false;  // means invalid neighborhood

            currColumn++;
        }

    }

    return validNeighborhood;
}

//======================================================================================================================
/**
 * @brief checks if the insertion of a word causes a valid vertical neighborhood
 * @param col - column to check
 * @param coords - word starting positions
 * @param word - word entered by the user
 * @return true if the vertical neighborhood is valid, false otherwise
 */
bool
Board::isValidVerticalNeighborhood(std::vector<char> col, std::pair<char, char> coords, const std::string &word) const {

    auto wordLength = static_cast<int> (word.size()), startLine = static_cast<int> (coords.first);
    int verticalLimit = (wordLength + startLine);
    bool validNeighborhood = true;

    for (int currLine = startLine - 1; currLine <= verticalLimit; currLine++) {

        std::string lettersInNeighborhood;  // temporary string to hold every letter in the neighborhood

        while (currLine >= 0 && currLine < static_cast<int>(m_numLines) && currLine <= verticalLimit
        && col.at(static_cast<size_t>(currLine)) != '0') {

            // found a letter in the neighborhood

            lettersInNeighborhood += col.at(static_cast<size_t>(currLine));

            if (lettersInNeighborhood.size() >= 2) return false;  // means invalid neighborhood

            currLine++;
        }

    }

    return validNeighborhood;
}

//======================================================================================================================
/**
 * @brief uses binary search check if a word is in the dictionary
 * @param word - the word to find
 * @return true if the word is in the dictionary, false otherwise
 */
bool Board::isInDictionary(const std::string &word) {
    return (std::binary_search(m_dictionary.begin(), m_dictionary.end(), word));
}

//======================================================================================================================
/**
 * @brief checks if a new larger word caused by crossing or superposition of words is valid
 * @param sequence - line/column to be checked
 * @param newWordDirection - the new word's direction
 * @return true if the addition is valid, otherwise false
 */
bool
Board::isValidAddition(std::vector<char> sequence, const char newWordDirection) {

    // the number of places to be checked depend on the direction of the newly inserted word
    size_t toCheck = newWordDirection == 'H' ? m_numCols : m_numLines;

    bool validAddition = true;


    for (size_t currentPlace = 0; currentPlace < toCheck; currentPlace++) {

        std::string formedWord{};  // string that temporarily holds the newly formed word

        while (sequence.at(currentPlace) != '0') {

            formedWord += sequence.at(currentPlace);  // found a letter, add it to the word

            if (currentPlace == toCheck - 1) break;  // reached the limit

            currentPlace++;
        }

        if (formedWord.size() > 1) { // found a word (note that the game does not allow 1 letter words)

            if (!isInDictionary(formedWord)) return false;  // an invalid word was formed

        }
    }

    return validAddition;
}

//======================================================================================================================
/**
 * @brief deletes a word that was temporarily inserted to be tested
 * @param coords - starting positions of the word
 * @param dir - direction of the word (vertical ou horizontal)
 * @param sequence - line/column before the word was inserted
 */
void Board::removeTemporaryWord(std::pair<char, char> coords, const char &dir, std::vector<char> sequence) {

    auto startLine = static_cast<size_t> (coords.first), startCol = static_cast<size_t> (coords.second);

    std::vector<char> line = getLine(startLine);
    std::vector<char> col = getCol(startCol);

    if (dir == 'H') {

        for (size_t i = 0; i < m_numCols; i++) {

            if (sequence.at(i) != line.at(i)) {
                                                      // compares the current line with the old line
                m_board.at(startLine).at(i) = '0';    // and deletes the letters where they are not equal
                                                      // restoring the original line
            }
        }

    } else if (dir == 'V') {

        for (size_t i = 0; i < m_numLines; i++) {

            if (sequence.at(i) != col.at(i)) {
                                                     // compares the current column with the old line
                m_board.at(i).at(startCol) = '0';    // and deletes the letters where they are not equal
                                                     //  restoring the original column
            }
        }

    }
}

//======================================================================================================================
/**
 * @brief checks if a word does not cross the board limits
 * @param word - the word
 * @param coords - the start of the word
 * @param dir - the direction of the word
 * @return true if the word stays withing the board limits, false otherwise
 */
bool Board::isInBoardLimits(const std::string &word, std::pair<char, char> coords, const char &dir) const {

    auto startLine = static_cast<size_t> (coords.first), startCol = static_cast<size_t> (coords.second);

    return !((dir == 'H' && (m_numCols - startCol) < word.size()) ||
             (dir == 'V' && (m_numLines - startLine) < word.size()));
}

//======================================================================================================================
/**
 * @brief check if the insertion of a word causes a bad word intersection on the board
 * @param word - the inserted word
 * @param coords - the starting position of the inserted word
 * @param dir - the direction of the word (horizontal (H) or vertical (V))
 * @return true if the word causes a problematic intersection, false otherwise
 */
bool Board::badIntersection(const std::string &word, std::pair<char, char> coords, const char &dir) {

    auto startLine = static_cast<size_t> (coords.first), startCol = static_cast<size_t> (coords.second);

    size_t wordLen = word.size();

    size_t horizontalLimit = (wordLen + startCol); // position (column) next to the last horizontal word position
    size_t verticalLimit = (wordLen + startLine); // position (line) next to the last vertical word position

    std::vector<char> line = getLine(startLine);
    std::vector<char> column = getCol(startCol);

    bool invalidIntersection = false;

    if (dir == 'H') {

        for (size_t i = startCol; i < horizontalLimit; i++) {

            if ((line.at(i) != '0') && (word.at(i - startCol) != line.at(i))) {

                invalidIntersection = true;
                break;
            }
        }

    } else if (dir == 'V') {

        for (size_t i = startLine; i < verticalLimit; i++) {

            if ((column.at(i) != '0') && (word.at(i - startLine) != column.at(i))) {

                invalidIntersection = true;
                break;
            }
        }

    }

    return invalidIntersection;

}

//======================================================================================================================
/**
 * @brief tests if the insertion of a word results in valid word limits across the board
 * @param word - word entered by the user
 * @param coords - word starting positions
 * @param dir - direction (vertical or horizontal)
 * @return false if the limits are not valid, otherwise true
 */
bool Board::validWordLimits(const std::string &word, std::pair<char, char> coords, const char &dir) {

    auto startLine = static_cast<size_t> (coords.first), startCol = static_cast<size_t> (coords.second);

    size_t hLimit = (word.length() + startCol);
    size_t vLimit = (word.length() + startLine);

    std::vector<char> line = getLine(startLine);
    std::vector<char> column = getCol(startCol);

    bool validLimits = true;

    if (dir == 'H' && (startCol != 0 || (hLimit) != m_numCols)) {

        if ((startCol == 0 && line.at(hLimit) != '0') || ((hLimit) == m_numCols && line.at(startCol - 1) != '0')
            || ((startCol != 0 && (hLimit) != m_numCols) && (line.at(hLimit) != '0' || line.at(startCol - 1) != '0'))) {

            insertWord(word, coords, dir);  // temporarily insert the word to check if it causes any problem

            std::vector<char> tempLine = getLine(startLine);  // get the line updated with the temporary word

            if (!isValidAddition(tempLine, 'H')) {

                validLimits = false;
            }

            removeTemporaryWord(coords, dir, line); // restore the line to how it was
        }

    } else if (dir == 'V' && (startLine != 0 || (vLimit) != m_numLines)) {

        if ((startLine == 0 && column.at(vLimit) != '0') || (vLimit == m_numLines && column.at(startLine - 1) != '0')
            || ((startLine != 0 && (vLimit) != m_numLines) &&
                (column.at(vLimit) != '0' || column.at(startLine - 1) != '0'))) {

            insertWord(word, coords, dir);  // temporarily insert the word to check if it causes any problem

            std::vector<char> tempColumn = getCol(startCol); // get the column updated with the temporary word

            if (!isValidAddition(tempColumn,'V')) {

                validLimits = false;
            }
            removeTemporaryWord(coords, dir, column);  // restore the line to how it was
        }

    }

    return validLimits;
}

//======================================================================================================================
/**
 * @brief checks the neighborhood of the inserted word for any problem it might cause
 * @param word - word entered by the user
 * @param coords - word starting positions
 * @param dir - direction (vertical or horizontal)
 * @return true if the insertion of the word causes no problematic neighborhoods, false otherwise
 */
bool Board::validWordNeighborhood(const std::string &word, std::pair<char, char> coords, const char &dir) {

    auto startLine = static_cast<size_t> (coords.first), startCol = static_cast<size_t> (coords.second);

    bool check = true;

    if (dir == 'H' && ((startLine != 0 || (startLine + 1) != m_numLines))) {

        if ((startLine + 1) != m_numLines) {

            std::vector<char> nextLine = getLine(startLine + 1);
            check = isValidHorizontalNeighborhood(nextLine, coords, word);               // check inferior neighborhood
        }
        if (startLine != 0 && check) {

            std::vector<char> prevLine = getLine(startLine - 1);
            check = isValidHorizontalNeighborhood(prevLine, coords, word);               // check superior neighborhood
        }

    } else if (dir == 'V' && (startCol != 0 || ((startCol + 1) != m_numCols))) {

        if ((startCol + 1) != m_numCols) {

            std::vector<char> nextCol = getCol(startCol + 1);
            check = isValidVerticalNeighborhood(nextCol, coords, word);                 // check right neighborhood
        }
        if (startCol != 0 && check) {

            std::vector<char> prevCol = getCol(startCol - 1);
            check = isValidVerticalNeighborhood(prevCol, coords, word);                // check left neighborhood
        }
    }

    return check;
}

//======================================================================================================================
/**
 * @brief searches the board for possible word addition cases (eg: have egg and make it eggs) and validates them
 * @param word - word entered by the user
 * @param coords - word's starting position
 * @param dir - word's direction (vertical or horizontal)
 * @return true if inserting a word on top of another causes no problems, false otherwise
 */
bool Board::causesValidWordAdditions(const std::string &word, std::pair<char, char> coords, const char &dir) {

    auto startLine = static_cast<size_t> (coords.first), startCol = static_cast<size_t> (coords.second);

    size_t horizontalLimit = (word.length() + startCol);
    size_t verticalLimit = (word.length() + startLine);

    bool valid = true;

    if (dir == 'H') {

        for (size_t i = startCol; i < horizontalLimit; i++) {

            std::vector<char> col = getCol(i);
            char saveBoardPos = col.at(startLine);          // save the state of that place in the board
            col.at(startLine) = word.at(i - startCol);   // changes it temporally to the new inserted letter

            if (!isValidAddition(col, 'V')) {      // check if any problem raises, and set valid accordingly
                valid = false;
            }

            col.at(startLine) = saveBoardPos;               // return to the original state
        }

    } else if (dir == 'V') {

        for (size_t i = startLine; i < verticalLimit; i++) {

            std::vector<char> line = getLine(i);
            char saveBoardPos = line.at(startCol);          // save the state of that place in the board
            line.at(startCol) = word.at(i - startLine);  // changes it temporally to the new inserted letter

            if (!isValidAddition(line, 'H')) {     // check if any problem raises, and set valid accordingly
                valid = false;
            }

            line.at(startCol) = saveBoardPos;               // return to the original state
        }
    }

    return valid;
}

//======================================================================================================================
