#ifndef MASKVERIFIER_H
#define MASKVERIFIER_H

#include "PNG.h"

class MaskVerifier {
public:
    bool verifySameShade(const Pixel& img1Pixel, const Pixel& maskPixel,
                         const Pixel& averageBackground, int tolerance);
};

#endif