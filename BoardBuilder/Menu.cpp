//
// Created by ritam on 27/04/20.
//

#include "Menu.h"
#include <iostream>
#include <sstream>
#include <vector>


Menu::Menu(const std::string &msg, const std::string &invalid,
           const std::unordered_map<std::string, std::function<void()>> &commands) {

    message_ = msg;
    invalidMessage_ = invalid;
    commands_ = commands;


}

void Menu::operator()() const {

    int count = 1;

    std::cout << message_;
    for (const auto &cmd : commands_) {
        std::cout << '[' << count << ']' << " " << cmd.first  << '\n';
        count ++;
    }


    int choiceNum;  // user choice number
    std::string userChoice;  // user choice name
    std::vector<std::string> options{};

    for (const auto& i : commands_) {  // get keys from map
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
                std::cout << invalidMessage_;
                std::cin >> choiceNum;
            }
        } else if (choiceNum < 1 || choiceNum > options.size()) {
            std::cout << invalidMessage_;
            std::cin >> choiceNum;
        } else {
            break;
        }
    }


    userChoice = options.at(choiceNum - 1);

    commands_.at(userChoice)();
}
