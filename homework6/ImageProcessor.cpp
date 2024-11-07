#include "ImageProcessor.h"

Pixel ImageProcessor::computeBackgroundPixel(const PNG& img1, const PNG& mask,
                                             int startRow, int startCol,
                                             int maxRow, int maxCol) {
    const Pixel Black{.rgba = 0xff'00'00'00U};
    int red = 0, blue = 0, green = 0, count = 0;
    for (int row = 0; row < maxRow; row++) {
        for (int col = 0; col < maxCol; col++) {
            if (mask.getPixel(row, col).rgba == Black.rgba) {
                const auto pix = img1.getPixel(row + startRow, col + startCol);
                red += pix.color.red;
                green += pix.color.green;
                blue += pix.color.blue;
                count++;
            }
        }
    }
    if (count == 0) {
        return {.color = {0, 0, 0, 0}};
    }
    return {.color = {static_cast<unsigned char>(red / count),
                      static_cast<unsigned char>(green / count),
                      static_cast<unsigned char>(blue / count), 0}};
}

void ImageProcessor::drawBox(PNG& png, int row, int col, int width, int height) {
    for (int i = 0; i < width; i++) {
        png.setRed(row, col + i);
        png.setRed(row + height - 1, col + i);
    }
    for (int i = 0; i < height; i++) {
        png.setRed(row + i, col);
        png.setRed(row + i, col + width - 1);
    }
}