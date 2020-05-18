
#ifndef BOARDBUILDER_MENU_H
#define BOARDBUILDER_MENU_H

#include <string>
#include <map>
#include <functional>


class Menu {
public:
    Menu(std::string msg, std::string invalid,
         std::map<std::string, std::function<void()>> commands);

    void operator()() const;

private:
    std::string _message;
    std::string _invalidMessage;
    std::map<std::string, std::function<void()>> _commands;
};


#endif //BOARDBUILDER_MENU_H
