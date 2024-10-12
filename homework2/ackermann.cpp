// Copyright (C) 2016 raodm@miamiOH.edu

// A simple implementation of an Ackermann method.

#include <iostream>
#include <string>

int Ack(const int m, const int n) {
    if (m > 0) {
        if (n > 0) {
            return Ack(m - 1, Ack(m, n - 1));
        } else {
            return Ack(m - 1, 1);
        }
    } else {
        return n + 1;
    }
}

int main(int argc, char *argv[]) {
    const int n = ((argc == 2) ? std::stoi(argv[1]) : 1);
    std::cout << "Ack(3," << n << "): " << Ack(3, n) << std::endl;
    return 0;
}
