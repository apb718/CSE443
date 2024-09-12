// Copyright 2024 Alec Byrd
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>

// It is ok to use the following namespace delarations in C++ source
// files only. They must never be used in header files.
using namespace std;
using namespace std::string_literals;

int main(int argc, char *argv[]) {
    std::ifstream in(argv[1]);
    int max, max2nd, val;
    in >> max >> max2nd;

    if (max < max2nd) {
        std::swap(max, max2nd);
    }

    while (in >> val) {
        if (max < val) {
            max2nd = max;
            max = val;
        } else if ((val < max) && (max2nd < val)) {
            max2nd = val;
        }
    }
    std::cout << "2nd max = " << max2nd << std::endl;
    
    return 0;
}

// End of source code
