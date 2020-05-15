
#ifndef PROGT2_UTILITY_H
#define PROGT2_UTILITY_H

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

void setBackgroundColor(unsigned colour);

void clearScreen();

void searchFile(std::string &fileName);

void printMessage(const std::string& message, unsigned colour);

void goToXY(int x, int y);

void clearLineAndGoUp();



#endif //PROGT2_UTILITY_H
