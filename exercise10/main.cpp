/* 
 * A C++ program to explore vectorization (that is use SSE) support
 * in G++
 * 
 * File:   exercise9.cpp
 * Author: raodm
 *
 * Copyright (C) raodm@miamiOH.edu on March 5, 2017, 1:38 PM
 */

#include <iostream>
#include <vector>

// An alias for a vector of floating point numbers
using FloatVec = std::vector<float>;

/** SAXPY is an acronym for "Single-Precision A.X + Y. 
 *  That is: Z = A * x + Y. 
 * 
 * SAXPY is a very common operation in computer graphics, games, image
 * processing, and linear algebra. It is a highly vectorizable operation 
 * because -- each element in the vector can be operated upon in parallel
 * independent of other elements.
 */
void saxpy(const FloatVec& X, FloatVec& Y, const float alpha = 0.1) {
    for (size_t i = 0; i < X.size(); i++) {
        Y[i] += alpha * X[i];
    }
}

/** Simple method to make a list of single-precision floating-point 
 * numbers from 0, 1, ..., maxVal-1 (this essentially std::iota)
 * 
 * @param maxVal The size of the array to return.
 * 
 * @return This method returns a vector of floating-point numbers
 */
FloatVec makeList(int maxVal) {
    FloatVec list(maxVal);
    for (int i = 0; (i < maxVal); i++) {
        list[i] = i;
    }
    return list;
}

/**
 * Simple method to determine sum of numbers in a list.
 * 
 * @param x The list of numbers to be summed.
 * 
 * @return The sum of numbers in the list.
 */
float sum(const FloatVec& list) {
    float total = 0;
    for (auto f : list) {
        total += f;
    }
    return total;
}

/*
 * A simple top-level main method for testing. It really does not
 * do any special operations.
 */
int main() {
    const int ListSize = 128;
    FloatVec list1 = makeList(ListSize), list2 = makeList(ListSize);
    saxpy(list1, list2);
    std::cout << "sum: " << sum(list1) << std::endl;
    return 0;
}

// End of source code
