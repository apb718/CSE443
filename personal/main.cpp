#include <iostream>
// #include <string>
// #include <fstream>
// #include <sstream>
// #include <iomanip>
// #include <utility>
// #include <vector>
// #include <algorithm>
// #include <numeric>
// #include <unordered_map>
#include <bits/stdc++.h>

// It is ok to use the following namespace delarations in C++ source
// files only. They must never be used in header files.
using namespace std;
using namespace std::string_literals;

long long runLog() {
    long long total = 0;
    for (unsigned int i = 0; i <100000000; i++) {
        total +=std::log(i);
    }
    return  total;
}

int main(int argc, char *argv[]) {
    
    std::cout << runLog() << std::endl;
    return 0;
}



// End of source code
