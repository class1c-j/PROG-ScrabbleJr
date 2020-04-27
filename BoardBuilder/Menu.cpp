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

    int count = 1;

    std::cout << message_;
    for (const auto &cmd : commands_) {
        std::cout << '[' << count << ']' << " " << cmd.first  << '\n';
        count ++;
    }

}

void Menu::operator()() const {
    int choiceNum;  // user choice number
    std::string userChoice;  // user choice name
    std::vector<std::string> options{};

    for (const auto& i : commands_) {  // get keys from map
        options.push_back(i.first);
    }


    while (std::cin >> choiceNum) {
        if (choiceNum < 1 || choiceNum > options.size()) {
            std::cout << invalidMessage_;
        } else {
            break;
        }
    }

    userChoice = options.at(choiceNum - 1);

    commands_.at(userChoice)();
}
