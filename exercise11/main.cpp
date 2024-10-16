// Copyright (C) 2016 raodm@miamiOH.edu

/** A program to compute factors of a given list of numbers in
    parallel using OpenMP
*/

#include <omp.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <string>

// A shortcut to refer to a vector of integers
using IntVec = std::vector<int>;

// Forward declaration to method that computes number of factors
int getFactorCount(int num); 

// Compute factors for a list of numbers in a data parallel manner
IntVec getFactors(const IntVec& numList) {
    IntVec factList(numList.size());
    #pragma omp parallel 
    {
        const int iterationsPerThread = numList.size() / omp_get_num_threads(); 
        const size_t startIndex = omp_get_thread_num() * iterationsPerThread;
        size_t endIndex = startIndex + iterationsPerThread;
        if (omp_get_num_threads() == omp_get_thread_num() + 1) {
            endIndex = numList.size();
        }
        for (size_t idx = startIndex; idx < endIndex; idx++) {
                factList[idx] = getFactorCount(numList[idx]);
        }
    }
    
    return factList;
}

//---------------------------------------------------------------------
//    DO   NOT   MODIFY   CODE   BELOW   THIS   LINE
//---------------------------------------------------------------------

// Returns the number of factors for a given number.
int getFactorCount(int num) {
    int factCount     = 0;
    for (int fact = 1; (fact <= num); fact++) {
        if (num % fact == 0) {
            factCount++;
        }
    }
    return factCount;
}

int main() {
    IntVec numList(100);
    std::iota(numList.begin(), numList.end(), 100000000);
    const IntVec factList = getFactors(numList);
    const int total = std::accumulate(factList.begin(), factList.end(), 0);
    std::cout << "Total number of factors: " << total << std::endl;
    return 0;
}
