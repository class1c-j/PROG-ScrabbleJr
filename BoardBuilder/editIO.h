//
// Created by ritam on 27/04/20.
//

#ifndef BOARDBUILDER_EDITIO_H
#define BOARDBUILDER_EDITIO_H

#include <vector>
#include "Board.h"

void readDimensions(unsigned &lines, unsigned &cols);

void readCoordinates(std::pair<char, char> &coords, const Board &board);

void readOrientation(char &orientation);

void readWord(std::string &word);

void readFileName(std::string &fileName);

void clearScreen();

void searchFile(std::string &fileName);

void printMessage(const std::string& message, unsigned colour);

#endif //BOARDBUILDER_EDITIO_H
