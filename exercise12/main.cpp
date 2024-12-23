/* 
 * File:   exercise11.cpp
 * Author: raodm
 *
 * Copyright (C) 2017 raodm@miamiOH.edu
 */

#include <omp.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

// Alias to streamline code below.
using IntVec = std::vector<unsigned int>;

// Forward declaration for popcount method defined further below
int popcount(unsigned int);

long countBits(const IntVec& numList) {
    long bitcount = 0;
#pragma omp parallel
  {  // fork
     const int iterPerThr = numList.size() / omp_get_num_threads();
     const int startIndex = omp_get_thread_num() * iterPerThr;
     int endIndex   = startIndex + iterPerThr;
     if (omp_get_num_threads() == omp_get_thread_num() + 1) {
         // Handle case where numList.size() is not evenly divisible
         // by number of threads
         endIndex = numList.size();
     }
     for (int idx = startIndex; (idx < endIndex); idx++) {
        #pragma omp critical 
        {
            bitcount += popcount(numList[idx]);
        }
     }
  }  // join
  return bitcount;
}

//-------------------------------------------------------------
//  DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
//-------------------------------------------------------------

/**
 * Helper method to count number of '1' bits in a given
 * unsigned integer.
 * 
 * @param value The value in which number of '1' bits should be
 * counted.
 * 
 * @return The number of '1' bits.
 */
int popcount(unsigned int value) {
    int bitcount = 0;  // number of '1' bits
    while (value > 0) {
        if (value % 2 == 1) {
            bitcount++;   // track 1 bits
        }
        value /= 2;  // remove last bit.
    }
    return bitcount;
}

/*
 * The main method that is primarily used for testing the 
 * countBits method in this program. 
 */
int main() {
    IntVec numList(50000000);
    std::iota(numList.begin(), numList.end(), 0);
    std::cout << "Bit count: " << countBits(numList) << std::endl;
    return 0;
}

