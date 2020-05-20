
#ifndef PROGT2_UTILS_H
#define PROGT2_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>

// Setup ANSI sequence support for windows 10 computers
#ifdef _WIN32
#include <windows.h>
int windowsSetup();
#endif

void clearScreen();

void searchBoard(std::string &fileName);

void printMessage(const std::string& message, unsigned colour);

void goToXY(size_t x, size_t y);

void clearLineAndGoUp();

void clearFromCursor();

void removeCarriageReturn(std::string &string);


#endif //PROGT2_UTILS_H
