
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
#include "../CommonFiles/utility.h"

void readLetter(char &c);

void readCoordinates(std::pair<char, char> &coords, const Board &board);

void readNumberPlayers(size_t &number);

std::vector<std::string> readPlayersNames(unsigned number);
