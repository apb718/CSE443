// Copyright (C) 2016 raodm@miamiOH.edu

#include <iostream>
#include <vector>
#include <sys/time.h>

// Program to benchmark the effectiveness of cache access patterns

/** The matrix size that we are working with. This value can be changed
 * depending on the machine such that it fits in cache.
 */
const int MatrixSize = 5000;

/** The matrix using which the tests are going to be performed. It is
 * placed in a namespace to avoid polluting the globals
 */
namespace mat {
    std::vector< std::vector<int> >
    matrix(MatrixSize, std::vector<int>(MatrixSize) );
};

/**
   Fills in the values into the matrix for testing purposes. This
   method is not really being benchmarked but does consume some time
   to run.
*/
void generateMatrix() {
    for (int col = 0; (col < MatrixSize); col++) {
        for (int row = 0; (row < MatrixSize); row++) {
            mat::matrix[row][col] = row + col;
        }
    }
}


/** This approach for computing sum of each column of the given
 * matrix.  The code is structured as:
 *
 * For each column in the matrix compute sum of all rows in the column
 * of the matrix.
 */
void columnSum_colMajor(std::vector<int> &colSum) {
    colSum.clear();
    colSum.resize(MatrixSize);
    for (int col = 0; (col < MatrixSize); col++) {
        for (int row = 0; (row < MatrixSize); row++) {
            colSum[col] += mat::matrix[row][col];
        }
    }
}

/** This approach for computing sum of each column of the given
 * matrix.  The code is structured as:
 *
 * For each row in the matrix update current sum for all columns in
 * the matrix
 */
void columnSum_rowMajor(std::vector<int> &colSum) {
    colSum.clear();
    colSum.resize(MatrixSize);
    for (int row = 0; (row < MatrixSize); row++) {
        for (int col = 0; (col < MatrixSize); col++) {
            colSum[col] += mat::matrix[row][col];
        }
    }
}

/**
 * The main function just calls the columnSum_colMajor or
 * columnSum_rowMajor methods depending on the 1st command-line
 * argument.
 *
 * \param[in] argc The number of command-line arguments. This program
 * requires just 1 command-line argument.
 *
 * \param[in] argv The actual command-line arguments. If the
 * command-line argument is "colMajor" then the columnSum_colMajor
 * method is called. Otherwise the columnSum_rowMajor is called.
 *
 * \return This method always returns zero.
 */
int main(int argc, char *argv[]) {
    generateMatrix();
    std::vector<int> colSum(MatrixSize);
    if ((argc > 1) && (argv[1] == std::string("colMajor"))) {
        for (int i = 0; (i < 100); i++) {
            columnSum_colMajor(colSum);
        }
    } else {
        for (int i = 0; (i < 100); i++) {
            columnSum_rowMajor(colSum);
        }
    }
    std::cout << "colSum[10] = " << colSum[10] << std::endl;
    return 0;
}

// End of source code.
