// Copyright Alec Byrd 2024
#ifndef IMAGESEARCH_H
#define IMAGESEARCH_H

#include "PNG.h"
#include "ImageProcessor.h"
#include "MaskVerifier.h"
#include "BoundingBox.h"
#include <vector>
#include <tuple>
#include <atomic>

class ImageSearch {
public:
    void search(const std::string& mainImageFile, const std::string& srchImageFile,
                const std::string& outImageFile, bool isMask = true,
                int matchPercent = 75, int tolerance = 32);

private:
    ImageProcessor imageProcessor;
    MaskVerifier maskVerifier;
    std::vector<std::pair<int, int>> found;
    int countOfMatches = 0;
};

#endif