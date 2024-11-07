#include "ImageSearch.h"

void ImageSearch::search(const std::string& mainImageFile, const std::string& srchImageFile,
                         const std::string& outImageFile, bool isMask,
                         int matchPercent, int tolerance) {
    PNG img1, mask, out;
    img1.load(mainImageFile);
    mask.load(srchImageFile);
    out = img1;

    int imgHeight = img1.getHeight();
    int imgWidth = img1.getWidth();
    int maskHeight = mask.getHeight();
    int maskWidth = mask.getWidth();

    std::vector<std::vector<std::pair<int, int>>> threadResults;

#pragma omp parallel
    {
        int threadNum = omp_get_thread_num();

#pragma omp single
        threadResults.resize(omp_get_num_threads());

#pragma omp for schedule(static)
        for (int i = 0; i <= imgHeight - maskHeight; i++) {
            for (int j = 0; j <= imgWidth - maskWidth; j++) {
                Pixel avgBackground = imageProcessor.computeBackgroundPixel(img1, mask, i, j, maskHeight, maskWidth);
                if (maskVerifier.verifySameShade(img1.getPixel(i, j), mask.getPixel(0, 0), avgBackground, tolerance)) {
                    threadResults[threadNum].emplace_back(i, j);
                }
            }
        }
    }

    // Merge and apply bounding boxes
    for (const auto& threadVec : threadResults) {
        for (const auto& coord : threadVec) {
            if (true /* Add bounding box logic */) {
                imageProcessor.drawBox(out, coord.first, coord.second, maskWidth, maskHeight);
            }
        }
    }

    out.write(outImageFile);
}