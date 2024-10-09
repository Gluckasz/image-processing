//
// Created by gluckasz on 10/7/24.
//

#ifndef RGBIMAGEPROCESSOR_H
#define RGBIMAGEPROCESSOR_H
#include "GrayscaleImageProcessor.h"


class RGBImageProcessor : public ImageProcessor {
public:
    RGBImageProcessor(Mat &image, int brightnessModVal, int contrastLinearModVal, float contrastGammaModVal);
    void modifyBrightness() override;
    void mofifyContrastLinear() override;
    void modifyContrastGamma() override;
    void negative() override;
    void flipHorizontally() override;
    void flipVertically() override;
    void flipDiagonally() override;
    Mat resize(float factor) override;
    Mat midpointFilter() override;
};



#endif //RGBIMAGEPROCESSOR_H
