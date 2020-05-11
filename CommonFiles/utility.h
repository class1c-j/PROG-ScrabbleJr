//
// Created by ritam on 04/05/20.
//

#ifndef PROGT2_UTILITY_H
#define PROGT2_UTILITY_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>

void setBackgroundColor(unsigned colour);

void clearScreen();

void searchFile(std::string &fileName);

void printMessage(const std::string& message, unsigned colour);

void gotoxy(int x, int y);

#endif //PROGT2_UTILITY_H
