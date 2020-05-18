
#include "Menu.h"
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <limits>


Menu::Menu(std::string msg, std::string invalid, std::map<std::string, std::function<void()>> commands)
           : _message(std::move(msg)), _invalidMessage(std::move(invalid)), _commands(std::move(commands)) {

}


/**
 * @brief shows a menu with all the options. user selects between the given numbers to call the corresponding function
 */
void Menu::operator()() const {

    int count = 1;  // to show the numbers of each option on the screen

    std::cout << _message;
    for (const auto &cmd : _commands) {
        std::cout << '(' << count << ')' << " - " << cmd.first << '\n';
        count++;
    }


    size_t choiceNum{};  // user choice number
    std::string userChoice;  // user choice name

    std::vector<std::string> options{};

    for (const auto &i : _commands) {  // get keys from map
        options.push_back(i.first);
    }

    std::cin >> choiceNum;

    while (true) {
        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "User chose to close the input.\n";
                break;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << _invalidMessage;
                std::cin >> choiceNum;
            }
        } else if (choiceNum < 1 || choiceNum > options.size()) {
            std::cout << _invalidMessage;
            std::cin >> choiceNum;
        } else {
            break;
        }
    }

    userChoice = options.at(choiceNum - 1);

    _commands.at(userChoice)();
}
