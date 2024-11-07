/**
 * A simple matrix multiplication program to explore the effectiveness
 * of OpenMP offload to a GPU.
 *
 * Copyright (C) 2021 raodm@miamioh.edu
 */

#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>

/** The data type used to represent real numbers.  This can be changed
 * to float, double, long double, etc. to experiment with
 * different data types and their impacts on runtime and percision.
 */
using Real = double;

/** Alias to streamline the code below */
using RealVec = std::vector<Real>;

/** A simple matrix class that justs uses a flat vector for storing
 * the data. The flat representation is needed to ease mapping the
 * data to the GPU.
 *
 *   \note Since the matrix representation is flat, an entry at (row,
 *   col) is accessed via the formula row * width + col.
*/
class SimpleMatrix : public RealVec {
public:
    /**
     * A simple constructor to create a matrix.
     *
     * \param[in] width The width or number of columns in the matrix.
     *
     * \param[in] height The height or number of rows in the matrix.
     *
     * \param[in] initVal An initial value with which each entry in
     * the matrix is initialized.
     */
    SimpleMatrix(int width = 0, int height = 0, Real initVal = 0) :
        RealVec(width * height, initVal), width(width), height(height) {}

    /**
     * Simple method to return the diagonal sum for this matrix.
     *
     * @return The diagonal sum for this matrix.
     */
    Real diagSum() const;

    /**
     * Performs the dot product of two matrices. This method has a
     * O(n^3) time complexity.
     *
     * \param[in] rhs The other matrix to be used.  This matrix must
     * have the same number of rows as the number of columns in this
     * matrix.  Otherwise this method throws an excpetion.
     *
     * \param[out] The resulting matrix in which each value has been
     * computed must be stored.  This method assumes that the result
     * matrix is of the correct size.
     */
    void dot(const SimpleMatrix& m2, SimpleMatrix& result) const;
    
private:
    /** The width or number of columns in the matrix.  This value is
        set when the matrix is instantiated and is never changed. */
    int width;

    /** The width or number of columns in the matrix.  This value is
        set when the matrix is instantiated and is never changed. */    
    int height;
};


// Returns the diagonal sum for a given matrix.
Real SimpleMatrix::diagSum() const {
    const auto max  = std::max(width, height);
    const auto vals = data();
    Real sum = 0;
    for (int i = 0; (i < max); i++) {
        // Convert the row, column to the linear array
        sum += vals[i * width + i];
    }
    return sum;
}

// Performs dot product of two matrices (assuming the matrices have
// correct dimensions)
void SimpleMatrix::dot(const SimpleMatrix& m2, SimpleMatrix& result) const {
    // Ensure the dimensions of m1 and m2 are consistent
    assert(width == m2.height);

    // Some intermediate variables to ease offloading to GPU.
    const auto m1Data = data(), m2Data = m2.data();
    auto resData = result.data();
    const auto m2Width = m2.width, m1Width = width, m1Height = height;
    
    // Ensure dimension of the result matrix is correct
    assert(result.height == m1Height && result.width == m2Width);
    
    // Do the actual matrix multiplication..
#pragma omp target map(to: m1Data[0:size()], m2Data[0:m2.size()]),      \
    map(to: m1Height, m2Width, m1Width), map(from: resData[0:result.size()])
#pragma omp teams distribute parallel for
    for (int row = 0; (row < m1Height); row++) {
        for (int col = 0; (col < m2Width); col++) {
            Real sum = 0;
            for (int i = 0; (i < m1Width); i++) {
                sum += m1Data[row * m2Width + i] * m2Data[i * m2Width + col];
            }
            // Store the result in an appropriate entry
            resData[row * m2Width + col] = sum;
        }
    }
}

/**
 * The main function serves as a test harness to explore the time
 * taken for matrix multiplication with different sized matrices.
 *
 * \param[in] argc The number of command-line arguments.  This program
 * uses only one optional command-line argument.
 *
 * \param[in] argv The command-line arguments.  This program assumes
 * that the first argument is the size of the square-matrix to be used
 * for testing.
 */
int main(int argc, char *argv[]) {
    // Find dimension based on command-line arguments.
    const int dim = (argc > 1 ? std::atoi(argv[1]) : 100);
    std::cout << "Working with matrices of dimension: " << dim << ".\n";
    SimpleMatrix m1(dim, dim, 1),  m2(dim, dim, 2), m3(dim, dim);

    // Time the run 25 reps of matrix multiplication
    for (int rep = 0; (rep < 25); rep++) {
        m1.dot(m2, m3);
    }
}
