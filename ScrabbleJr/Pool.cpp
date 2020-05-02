//
// Created by ritam on 01/05/20.
//

#include "Pool.h"

Pool::Pool() = default;

Pool::Pool(const std::vector<std::string>& wordsBoard) {
    for (const auto& word : wordsBoard) {
        for (const auto& letter : word) {
            content.push_back(letter);
        }
    }
}

std::vector<char> Pool::getContent() {
    return content;
}

void Pool::shuffle() {
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(content), std::end(content), rng);
}