// Copyright Alec Byrd 2024
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <omp.h>
#include "PNG.h"

// It is ok to use the following namespace delarations in C++ source
// files only. They must never be used in header files.
using namespace std;
using namespace std::string_literals;

/**
 * This is the top-level method that is called from the main method to 
 * perform the necessary image search operation. 
 * 
 * \param[in] mainImageFile The PNG image in which the specified searchImage 
 * is to be found and marked (for example, this will be "Flag_of_the_US.png")
 * 
 * \param[in] srchImageFile The PNG sub-image for which we will be searching
 * in the main image (for example, this will be "star.png" or "start_mask.png") 
 * 
 * \param[in] outImageFile The output file to which the mainImageFile file is 
 * written with search image file highlighted.
 * 
 * \param[in] isMask If this flag is true then the searchImageFile should 
 * be deemed as a "mask". The default value is false.
 * 
 * \param[in] matchPercent The percentage of pixels in the mainImage and
 * searchImage that must match in order for a region in the mainImage to be
 * deemed a match.
 * 
 * \param[in] tolerance The absolute acceptable difference between each color
 * channel when comparing  
 */
std::vector<std::pair<int, int>> found;

// total matches
int countOfMatches = 0;
bool debug = false;  // debug flag
int debugRow = 0;
int debugCol = 0;

// struct used to make sure no overlapping/extra lines
struct BoundingBox {  // struct generated with ChatGPT-4o
    int topRow;
    int leftCol;
    int bottomRow;
    int rightCol;
    
    // Constructor for easy creation
    BoundingBox(int row, int col, int height, int width)
        : topRow(row), leftCol(col), 
          bottomRow(row + height), rightCol(col + width) {}
    
    // Check if two bounding boxes overlap
    bool overlapsWith(const BoundingBox& other) const {
    return !(bottomRow < other.topRow ||   // Above (strict inequality)
             topRow > other.bottomRow ||   // Below (strict inequality)
             rightCol < other.leftCol ||   // Left (strict inequality)
             leftCol > other.rightCol);    // Right (strict inequality)
    }
};

void 
drawBox(PNG& png, int row, int col, int width, int height) {
    for (int i = 0; i < width; i++) {
        png.setRed(row, col+i);
        png.setRed(row+height-1, col+i);
    }
    for (int i = 0; i < height; i++) {
        png.setRed(row + i, col);
        png.setRed(row + i, col + width -1);
    }
}

Pixel
computeBackgroundPixel(const PNG& img1, const PNG& mask,
                    const int startRow, const int startCol, 
                    const int maxRow, const int maxCol) {
    const Pixel Black{ .rgba = 0xff'00'00'00U};
    int red = 0, blue = 0, green = 0, count = 0;
    for (int row = 0; (row < maxRow); row++) {
        for (int col = 0; (col < maxCol); col++) {
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
     const unsigned char avgRed = (red / count), 
                         avgGreen = (green / count),
                         avgBlue = (blue / count);
    // if (debug && startRow == debugRow &&  startCol == debugCol) {
    //     std::cout << startRow << " " << startCol << " "
    //     << red / count << " " << green / count << " "
    //     << blue / count << std::endl;
    //     // exit(1);
    // }
     return {.color = {avgRed, avgGreen, avgBlue, 0}};
}


// Method used to verify each of the pixels against the background to
// validate whether or not it fits mask
bool
verifySameShade(const Pixel& img1Pixel, const Pixel& maskPixel, 
                const Pixel& averageBackground, const int tolerance,
                int row, int col, int maskRow, int maskCol) {
    const Pixel Black{ .rgba = 0xff'00'00'00U }; 
    const Pixel White{ .rgba = 0xff'ff'ff'ffU }; 
    bool sameShade = 
      std::abs(img1Pixel.color.red - averageBackground.color.red) < tolerance &&
      std::abs(img1Pixel.color.green - averageBackground.color.green) 
        < tolerance &&
      std::abs(img1Pixel.color.blue - averageBackground.color.blue) < tolerance;
    // if (false) {
    //     // std::cout << "Comparing Pixels:" << std::endl;
    //     std::cout << maskRow << "\t" << maskCol << "\t" << "(" << 
    //     static_cast<int>(img1Pixel.color.red)
    //       << ", " << static_cast<int>(img1Pixel.color.green)
    //       << ", " << static_cast<int>(img1Pixel.color.blue) << ")\t" 
    //       << "(" << static_cast<int>(maskPixel.color.red)
    //       << ", " << static_cast<int>(maskPixel.color.green)
    //       << ", " << static_cast<int>(maskPixel.color.blue)<<
    //        ")\t" << sameShade << std::endl;
    //     // std::cout << "Returning " << out << std::endl << std::endl;
    // }                  
    // if mask is black
    if (maskPixel.rgba == Black.rgba) {
        if (sameShade) { return true;
        } else {
            return false;
        }
    } else if (maskPixel.rgba == White.rgba) {
        if (sameShade) {return false;
        } else { return true;
        }
    } else {
        if (sameShade) { return false;
        } else { return true;
        }
    }
}

// Adds values to vector if they fit bounding boxes without collisions
bool addToFoundVectors(const PNG& mask, const int row, const int col) {
    BoundingBox newBox(row, col, mask.getHeight(), mask.getWidth());

    // Check for overlaps in the found list
    for (const auto& coord : found) {
        BoundingBox existingBox(coord.first, coord.second, 
                        mask.getHeight(), mask.getWidth());
        if (newBox.overlapsWith(existingBox)) {
            return false;  // If an overlap is found, exit early and do not add
        }
    }

    // If no overlaps, add the new match to the found list
    found.push_back({row, col});
    return true;
}

// sub method to show check the netmatch to avoid compiler warning
bool checkNetMatch(double netMatch, const PNG& mask, int matchPercent, 
                   int startRow, int startCol) {
    if (netMatch > mask.getWidth() * mask.getHeight() * matchPercent / 100.0) {
        if (!addToFoundVectors(mask, startRow, startCol)) {
            return false;
        }
        countOfMatches++;
        return true;
    }
    return false;
}

// Checks the pixels and finds the % net match to compare to the match percent
bool checkPixels(const PNG& img1, const PNG& mask, const Pixel& avgBackground,
                int startRow, int startCol, int maxRow, int maxCol,
                int matchPercent, int tolerance) {
    const Pixel Black{ .rgba = 0xff'00'00'00U };
    const Pixel White{ .rgba = 0xff'ff'ff'ffU };
    int matchCount = 0, mismatchCount = 0;
    for (int row = 0; row < mask.getHeight() && 
         (startRow + row) < img1.getHeight(); row++) {
        for (int col = 0; col < mask.getWidth() && 
             (startCol + col) < img1.getWidth(); col++) {
            const auto imgPix = img1.getPixel(startRow + row, startCol + col);
            const auto maskPix = mask.getPixel(row, col);
            if (maskPix.rgba == Black.rgba || maskPix.rgba == White.rgba) {
                verifySameShade(imgPix, maskPix, avgBackground, tolerance, 
                startRow, startCol, row, col) ? matchCount++ : mismatchCount++;

            } else {
                verifySameShade(imgPix, maskPix, avgBackground, tolerance, 
               startRow, startCol, row, col) ? matchCount++ : mismatchCount++;
            }
        }
    }
    return checkNetMatch(matchCount - mismatchCount, mask, 
        matchPercent, startRow, startCol);
}


// Initial method to start searching the image.
void imageSearch(const std::string& mainImageFile,
                const std::string& srchImageFile, 
                const std::string& outImageFile, const bool isMask = true, 
                const int matchPercent = 75, const int tolerance = 32) {
    // Implement this method using various methods or even better
    // use an object-oriented approach.
    PNG img1, mask, out;
    img1.load(mainImageFile);
    mask.load(srchImageFile); out = img1;
    int maskWidth = mask.getWidth(), maskHeight = mask.getHeight();
    int imgWidth = img1.getWidth(), imgHeight = img1.getHeight();
    for (int i = 0; i <= imgHeight - maskHeight; i++) {
        for (int j = 0; j <= imgWidth - maskWidth; j++) {
            Pixel backgroundColor = computeBackgroundPixel(img1, mask,
                                            i, j, maskHeight, maskWidth);
            checkPixels(img1, mask, backgroundColor,
                    i, j, maskHeight, maskWidth, matchPercent, tolerance);
        }
    }
    for (std::pair<int, int> item : found) {
        std::cout << "sub-image matched at: " 
        << item.first << ", " << item.second 
        << ", " << item.first+ mask.getHeight()
        << ", " << item.second + mask.getWidth() << std::endl;
        drawBox(out, item.first, item.second, maskHeight, maskHeight);
    }
    // std::cout << "Before write" << std::endl;
    out.write(outImageFile);
    // std::cout << "After write" << std::endl;
}



/**
 * The main method simply checks for command-line arguments and then calls
 * the image search method in this file.
 * 
 * \param[in] argc The number of command-line arguments. This program
 * needs at least 3 command-line arguments.
 * 
 * \param[in] argv The actual command-line arguments in the following order:
 *    1. The main PNG file in which we will be searching for sub-images
 *    2. The sub-image or mask PNG file to be searched-for
 *    3. The file to which the resulting PNG image is to be written.
 *    4. Optional: Flag (True/False) to indicate if the sub-image is a mask 
 *       (deault: false)
 *    5. Optional: Number indicating required percentage of pixels to match
 *       (default is 75)
 *    6. Optiona: A tolerance value to be specified (default: 32)
 */
int main(int argc, char *argv[]) {
    if (argc < 4) {
        // Insufficient number of required parameters.
        std::cout << "Usage: " << argv[0] << " <MainPNGfile> <SearchPNGfile> "
                  << "<OutputPNGfile> [isMaskFlag] [match-percentage] "
                  << "[tolerance]\n";
        return 1;
    }
    const std::string True("true");
    // Call the method that starts off the image search with the necessary
    // parameters.
    imageSearch(argv[1], argv[2], argv[3],       // The 3 required PNG files
        (argc > 4 ? (True == argv[4]) : true),   // Optional mask flag
        (argc > 5 ? std::stoi(argv[5]) : 75),    // Optional percentMatch
        (argc > 6 ? std::stoi(argv[6]) : 32));   // Optional tolerance

    std::cout << "Number of matches: " << countOfMatches << std::endl;
    return 0;
}

// End of source code
