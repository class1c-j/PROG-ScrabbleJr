
#ifndef PROGT2_GAMEIO_H
#define PROGT2_GAMEIO_H

#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#endif

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <algorithm>
#include <string>
#include "../CommonFiles/Board.h"
#include "../CommonFiles/utility.h"

void readLetter(std::string &c, size_t horizontalPadding, size_t verticalPadding);

void readCoordinates(std::pair<char, char> &coords, const Board &board);

void readNumberPlayers(size_t &number);

std::vector<std::string> readPlayersNames(size_t number);

#endif //PROGT2_GAMEIO_H
