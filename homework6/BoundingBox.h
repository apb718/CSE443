// Copyright 2024 Alec Byrd
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <vector>

class BoundingBox {
public:
    int topRow, leftCol, bottomRow, rightCol;

    BoundingBox(int row, int col, int height, int width)
        : topRow(row), leftCol(col),
          bottomRow(row + height), rightCol(col + width) {}

    bool overlapsWith(const BoundingBox& other) const {
        return !(bottomRow < other.topRow ||   // Above
                 topRow > other.bottomRow ||   // Below
                 rightCol < other.leftCol ||   // Left
                 leftCol > other.rightCol);    // Right
    }
};

#endif
