
#ifndef PROGT2_GAMEIO_H
#define PROGT2_GAMEIO_H

#endif //PROGT2_GAMEIO_H

#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#endif

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include "../CommonFiles/Board.h"

void searchFile(std::string &fileName);

void readLetter(char &c);

void readCoordinates(std::pair<char, char> &coords, const Board &board);
