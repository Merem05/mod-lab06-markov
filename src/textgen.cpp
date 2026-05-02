// Copyright 2024

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "textgen.h"

unsigned int currentSeed = 0;

void setSeed(unsigned int seed) {
    currentSeed = seed;
    srand(seed);
}

void buildTable(const std::string& filename, statetab& table,
                prefix& startPrefix) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return;
    }

    std::vector<std::string> words;
    std::string word;
    while (file >> word) {
        words.push_back(word);
    }
    file.close();

    if (words.size() < NPREF + 1) {
        std::cerr << "Error: text is too short" << std::endl;
        return;
    }

    startPrefix.clear();
    for (int i = 0; i < NPREF; ++i) {
        startPrefix.push_back(words[i]);
    }

    for (size_t i = 0; i < words.size() - NPREF; ++i) {
        prefix current;
        for (int j = 0; j < NPREF; ++j) {
            current.push_back(words[i + j]);
        }
        std::string suffix = words[i + NPREF];
        table[current].push_back(suffix);
    }
}

std::string generate(const statetab& table, const prefix& startPrefix,
                     int maxGen) {
    if (table.empty()) {
        std::cerr << "Error: table is empty" << std::endl;
        return "";
    }

    if (currentSeed == 0) {
        srand(time(NULL));
    } else {
        srand(currentSeed);
    }

    std::string W1 = startPrefix[0];
    std::string W2 = startPrefix[1];

    std::string result = W1 + " " + W2;

    for (int step = 0; step < maxGen; ++step) {
        prefix key;
        key.push_back(W1);
        key.push_back(W2);

        statetab::const_iterator it = table.find(key);
        if (it == table.end() || it->second.empty()) {
            break;
        }

        const std::vector<std::string>& suffixes = it->second;
        unsigned int seed = rand();
        int randomIndex = rand_r(&seed) % suffixes.size();  // NOLINT
        std::string W3 = suffixes[randomIndex];

        result += " " + W3;

        W1 = W2;
        W2 = W3;
    }

    return result;
}

void saveToFile(const std::string& filename, const std::string& text) {
    std::string dir = "result";
    system(("mkdir " + dir + " 2>nul").c_str());

    std::string fullPath = dir + "/" + filename;
    std::ofstream file(fullPath);
    if (file.is_open()) {
        file << text;
        file.close();
        std::cout << "Text saved to " << fullPath << std::endl;
    } else {
        std::cerr << "Error: cannot save file " << fullPath << std::endl;
    }
}
