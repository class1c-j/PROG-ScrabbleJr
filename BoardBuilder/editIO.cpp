#define NOMINMAX

#include "editIO.h"

/**
 * @brief reads and validates (showing the appropriate errors) the chosen dimensions for the board
 * @param lines - the number of lines chosen
 * @param cols  - the number of columns chosen
 */
void readDimensions(unsigned& lines, unsigned& cols) {

    std::cout << "Number of lines ? ";
    std::cin >> lines;

    while (std::cin.fail() || lines > 20 || (std::cin.peek() != '\n' && !isdigit(std::cin.peek()))) {

        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cout << "User chose to close the input.\n";
                std::cout << "Please enter the number of lines again: ";
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Failed to read number. Try again: ";
            }
            std::cin >> lines;

        }
        else {

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "That is not a valid number of lines. Remember, maximum allowed is 20. Try again: ";
            std::cin >> lines;
        }

    }

    std::cout << "Number of columns ? ";
    std::cin >> cols;

    while (std::cin.fail() || cols > 20 || (std::cin.peek() != '\n' && !isdigit(std::cin.peek()))) {
        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cout << "User chose to close the input.\n";
                std::cout << "Please enter the number of columns again: ";
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Failed to read number. Try again: ";
            }
            std::cin >> cols;
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "That is not a valid number of columns. Remember, maximum allowed is 20. Try again: ";
            std::cin >> cols;
        }
    }
}
/**
 * @brief reads and validates the starting position of the word
 * @param coords - the coordinates of the starting position
 * @param board - the board (used to validate according to the dimensions)
 */
void readCoordinates(std::pair<char, char>& coords, const Board& board) {

    std::cout << "Starting position line ? ";
    std::cin >> coords.first;

    while (std::cin.fail() || !isalpha(coords.first) || static_cast<size_t>(coords.first - 65) >= board.getNumberLines() ||
           std::cin.peek() != '\n') {
        if (std::cin.eof()) {
            std::cin.clear();
            std::cout << "User chose to close input.\n";
            std::cout << "Please enter the starting position line again: ";
        }
        else {
            std::cout << "Invalid line choice. Try again: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin >> coords.first;
    }

    std::cout << "Starting position column ? ";
    std::cin >> coords.second;
    while (std::cin.fail() || !isalpha(coords.second) || static_cast<size_t>(coords.second - 97) >= board.getNumberCols() ||
           std::cin.peek() != '\n') {
        if (std::cin.eof()) {
            std::cin.clear();
            std::cout << "User chose to close input.\n";
            std::cout << "Please enter the starting position column again: ";
        }
        else {
            std::cout << "Invalid column choice. Try again: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin >> coords.second;
    }

    coords.first -= 'A';
    coords.second -= 'a';


}

/**
 * @brief reads and validates the direction of the word
 * @param orientation the direction of the word
 */
void readOrientation(char& orientation) {

    std::cout << "Orientation ? ";
    std::cin >> orientation;
    while (std::cin.fail() || (orientation != 'V' && orientation != 'H') || std::cin.peek() != '\n') {
        if (std::cin.eof()) {
            std::cin.clear();
            std::cout << "User chose to close input.\n";
            std::cout << "Please enter the orientation again: ";
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid orientation. Insert V for vertical or H for horizontal. Try again: ";
        }
        std::cin >> orientation;
    }

}

/**
 * @brief reads and validates the word introduced by the user
 * @param word - the word
 */
void readWord(std::string& word) {
    std::cout << "Word ? ";
    std::cin >> word;

    while (true) {

        while (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cout << "User chose to close input.\n";
                std::cout << "Please enter the word again: ";
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Remember, your word must not have any spaces. Try again: ";
            }
            std::cin >> word;
        }
        for (const char i : word) {
            if (!isalpha(i)) {
                std::cout << "Word must not include numbers or symbols. Try again: ";
                std::cin >> word;
            }
        }
        break;
    }

    for (char& i : word) {
        i = static_cast<char>(tolower(i));
    }

}



/**
 * @brief reads and validates (by trying to make a file) the desired file name
 * @param fileName the desired file name
 */
void readFileName(std::string &fileName) {
    std::cout
            << "Be aware of your operating system's naming limitations.\nWindows - no reserved names, no <>:\\/|?*,"
               " don't end with \' \' or \'.\'\nLinux - no \'/\' \n";

    std::cout << "File name (without extension) ? ";
    std::cin >> fileName;
    fileName += ".txt";


    while (true) {

        std::ofstream test(fileName);

        if (std::cin.fail() || !test) {
            std::cout << "Invalid file name. Try again: ";
            std::cin >> fileName;
            fileName = "../UserBoards/" + fileName + ".txt";
        } else {
            break;
        }

    }
}

