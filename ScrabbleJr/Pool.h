//
// Created by ritam on 01/05/20.
//

#ifndef PROGT2_POOL_H
#define PROGT2_POOL_H


#include <vector>
#include <string>
#include <algorithm>
#include <random>

class Pool {
public:
    Pool();

    explicit Pool(const std::vector<std::string>& wordsBoard);

    std::vector<char> getContent();

    void shuffle();

private:
    std::vector<char> content;
};


#endif //PROGT2_POOL_H
