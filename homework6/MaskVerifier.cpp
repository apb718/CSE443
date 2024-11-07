#include "MaskVerifier.h"

bool MaskVerifier::verifySameShade(const Pixel& img1Pixel, const Pixel& maskPixel,
                                   const Pixel& averageBackground, int tolerance) {
    const Pixel Black{.rgba = 0xff'00'00'00U};
    const Pixel White{.rgba = 0xff'ff'ff'ffU};
    bool sameShade = std::abs(img1Pixel.color.red - averageBackground.color.red) < tolerance &&
                     std::abs(img1Pixel.color.green - averageBackground.color.green) < tolerance &&
                     std::abs(img1Pixel.color.blue - averageBackground.color.blue) < tolerance;

    if (maskPixel.rgba == Black.rgba) {
        return sameShade;
    } else if (maskPixel.rgba == White.rgba) {
        return !sameShade;
    }
    return !sameShade;
}