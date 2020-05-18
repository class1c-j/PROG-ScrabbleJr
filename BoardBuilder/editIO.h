
#ifndef PROGT2_EDITIO
#define PROGT2_EDITIO

#include <vector>
#ifdef _WIN32  // Windows compatibility
#include <windows.h>
#include <cstdlib>
#endif
#include <limits>
#include <iostream>
#include <ios>
#include "../CommonFiles/Board.h"

void readDimensions(unsigned &lines, unsigned &cols);

void readCoordinates(std::pair<char, char> &coords, const Board &board);

void readOrientation(char &orientation);

void readWord(std::string &word);

void readFileName(std::string &fileName);

#endif // PROGT2_EDITIO