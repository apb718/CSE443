#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "PNG.h"
#include "BoundingBox.h"
#include <vector>

class ImageProcessor {
public:
    Pixel computeBackgroundPixel(const PNG& img1, const PNG& mask,
                                 int startRow, int startCol, int maxRow, int maxCol);

    void drawBox(PNG& png, int row, int col, int width, int height);
};

#endif