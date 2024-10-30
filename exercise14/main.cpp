// A simple program to generate a large set of random numbers and
// operate on them using different scheduling options.
//
// Copyright (C) raodm@miamiOH.edu

#include <omp.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <utility>

// A convenience typedef to hold a number and its number of prime factor
// counts
using NumFactCount = std::pair<long, int>;

// Forward declaration (aka prototype) to keep compiler happy
bool isPrime(const long);
int getPrimeFactorCount(const long);
NumFactCount getMaxElement(const std::vector<NumFactCount>& maxFactCount);
    
// This method determines the value (in given numList) that has the
// higest number of prime factors.  The parallelization strategy is
// pretty standard (the same approach used in previous exercises):
//    1. The numbers are evenly subdivided between threads
//    2. Each thread computes its local maximum value (into a shared
//       vector called maxFactCount)
//    3. After threads join, the master thread computes the overall maximum
//       from the local-maximum computed by each thread.
NumFactCount
findNumberWithMaxPrimeFactorCount(const std::vector<long>& numList) {
    // Can't set size of vector below until we know number of threads.
    std::vector<NumFactCount> maxFactCount;

#pragma omp parallel shared(numList, maxFactCount)
    {  // fork
#pragma omp critical
        if (omp_get_thread_num() == 0) {
            // Resize shared vector
            maxFactCount.resize(omp_get_num_threads());
        }
// Wait for thread 0 to finish its task
#pragma omp barrier

     int thrId = omp_get_thread_num();
#pragma omp for schedule(runtime)
        for (size_t i = 0; (i < numList.size()); i++) {
            const int prFactCount = getPrimeFactorCount(numList[i]);

            if (maxFactCount[thrId].second < prFactCount) {
                maxFactCount[thrId] = std::make_pair(numList[i], prFactCount);
            }  // if-statement
        }
    }  // join (waits for all threads to finish)
    
    // Find and return the maximum from the various threads.
    return getMaxElement(maxFactCount);
}

// ----------------------------------------------------------------------
//     DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
// ----------------------------------------------------------------------

NumFactCount
getMaxElement(const std::vector<NumFactCount>& maxFactCount) {
    return *max_element(maxFactCount.cbegin(), maxFactCount.cend(),
                        [](const NumFactCount& p1, const NumFactCount& p2) {
                            return p1.second < p2.second;
                        });
    // The above max_element algorithm essentially accomplishes the
    // following logic:

    // size_t max = 0;
    // for (size_t i = 0; (i < maxFactCount.size()); i++) {
    //    if (maxFactCount[max].second < maxFactCount[i].second) {
    //        max = i;
    //    }
    // }
    // return maxFactCount[max];
}

int getPrimeFactorCount(const long number) {
    int count = 0;
    for (long factor = 1; (factor < number); factor++) {
        if ((number % factor == 0) && isPrime(factor)) {
            count++;
        }
    }
    return count;
}

bool isPrime(const long number) {
    int MaxFactor = sqrt(number) + 1;
    for (int factor = 2; (factor < MaxFactor); factor++) {
        if (number % factor == 0) {
            return false;
        }
    }
    return true;
}

long generator() {
    static int count = 0;
    unsigned int seed = 0;
    const long digits = (count++ % 4 > 1 ? 1000L : 10000000L);
    return rand_r(&seed) % digits;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Specify count of numbers to generate "
                  << "as command-line argument.\n";
        return 1;
    }
    // Generate vector with random numbers
    std::vector<long> numList(std::stoi(argv[1]));
    std::generate(numList.begin(), numList.end(), generator);
    std::sort(numList.begin(), numList.end());
    // Compute the largest prime number in the list.
    const NumFactCount bigPrime = findNumberWithMaxPrimeFactorCount(numList);
    // Print it.
    std::cout << "Number with most number of prime factors: "
          << bigPrime.first  << ", prime factor count = "
          << bigPrime.second << std::endl;
    return 0;
}

// End of source code.

