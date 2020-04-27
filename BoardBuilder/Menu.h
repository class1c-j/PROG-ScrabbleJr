
#ifndef BOARDBUILDER_MENU_H
#define BOARDBUILDER_MENU_H

#include <string>
#include <unordered_map>
#include <functional>


class Menu {
public:
    Menu(const std::string &msg, const std::string &invalid,
         const std::unordered_map<std::string, std::function<void()>> &commands);

    void operator()() const;

private:
    std::string message_;
    std::string invalidMessage_;
    std::unordered_map<std::string, std::function<void()>> commands_;
};


#endif //BOARDBUILDER_MENU_H
