// Copyright Alec Byrd 2024
#include "ImageSearch.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <MainPNGfile> <SearchPNGfile> <OutputPNGfile>" 
                                          << "[isMask] [matchPercent] [tolerance]\n";
        return 1;
    }

    ImageSearch searcher;
    searcher.search(argv[1], argv[2], argv[3], 
                    argc > 4 ? argv[4] == std::string("true") : true, 
                    argc > 5 ? std::stoi(argv[5]) : 75, 
                    argc > 6 ? std::stoi(argv[6]) : 32);

    return 0;
}