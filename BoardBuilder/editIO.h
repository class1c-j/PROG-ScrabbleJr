//
// Created by ritam on 27/04/20.
//

#ifndef BOARDBUILDER_EDITIO_H
#define BOARDBUILDER_EDITIO_H

#include <vector>
#include "Board.h"

void readDimentions(unsigned &col, unsigned &lines);

void readCoordinates(std::pair<char, char> &coords, const Board &board);

void readOrientation(char &orientation);

void readWord(std::string &word);

#endif //BOARDBUILDER_EDITIO_H
