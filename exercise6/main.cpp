/** A simple sorting benchmark that can be compiled and run using
    multiple threads

    Copyright (C) 2021 raodm@miamioh.edu
*/

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    const size_t count = (argc > 1 ? std::stol(argv[1]) : 100'000'000);
    std::vector<int> vals(count);
    std::generate_n(vals.begin(), count, rand);
    std::sort(vals.begin(), vals.end());
    std::cout << "Mid value = " << vals.at(vals.size() / 2) << std::endl;
}
