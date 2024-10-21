// Copyright (C) 2021 raodm@miamioh.edu

#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <cassert>
#include <vector>
#include "Matrix.h"

Matrix::Matrix(const size_t row, const size_t col, const Val initVal) :
    std::vector<std::vector<Val>>(row, std::vector<Val>(col, initVal)) {
}

// Operator to write the matrix to a given output stream
std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    // Print the number of rows and columns to ease reading
    os << matrix.height() << " " << matrix.width() << '\n';
    // Print each entry to the output stream.
    for (auto& row : matrix) {
        for (auto& val : row) {
            os << val << " ";
        }
        // Print a new line at the end of each row just to format the
        // output a bit nicely.
        os << '\n';
    }
    return os;
}

// Operator to read the matrix to a given input stream.
std::istream& operator>>(std::istream& is, Matrix& matrix) {
    // Temporary variables to load matrix sizes
    int height, width;
    is >> height >> width;
    // Now initialize the destination matrix to ensure it is of the
    // correct dimension.
    matrix = Matrix(height, width);
    // Read each entry from the input stream.
    for (auto& row : matrix) {
        for (auto& val : row) {
            is >> val;
        }
    }
    return is;
}

Matrix Matrix::dot(const Matrix& rhs) const {
    // Ensure the dimensions are similar.
    assert(front().size() == rhs.size());
    // Setup the result matrix
    const auto mWidth = rhs.front().size(), width = front().size();
    Matrix result(size(), mWidth);
    // Do the actual matrix multiplication
    for (size_t row = 0; (row < size()); row++) {
        for (size_t col = 0; (col < mWidth); col++) {
            Val sum = 0;
            for (size_t i = 0; i < width; i += 4) {
                sum += (*this)[row][i] * rhs[i][col];
                if (i + 1 >= width) break;
                sum += (*this)[row][i + 1] * rhs[i + 1][col];
                if (i + 2 >= width) break;
                sum += (*this)[row][i + 2] * rhs[i + 2][col];
                if (i + 3 >= width) break;
                sum += (*this)[row][i + 3] * rhs[i + 3][col];
            }
            // Store the result in an appropriate entry
            result[row][col] = sum;
        }
    }
    // Return the computed result
    return result;
}

Matrix Matrix::transpose() const {
    // If the matrix is empty, then there is nothing much to do.
    if (empty()) {
        return *this;
    }

    // Create a result matrix that will be the transpose, with width
    // and height flipped.
    Matrix result(width(), height());
    // Now copy the values creating the transpose
    for (int row = 0; (row < height()); row++) {
        for (size_t col = 0; col < (size_t)width(); col += 4) {
            result[col][row] = (*this)[row][col];
            if (col + 1 >= (size_t)width()) break;
            result[col + 1][row] = (*this)[row][col + 1];
            if (col + 2 >= (size_t)width()) break;
            result[col + 2][row] = (*this)[row][col + 2];
            if (col + 3 >= (size_t)width()) break;
            result[col + 3][row] = (*this)[row][col + 3];
        }
    }
    // Return the resulting transpose.
    return result;
}

#endif
