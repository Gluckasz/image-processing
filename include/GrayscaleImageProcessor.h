//
// Created by gluckasz on 10/6/24.
//

#ifndef GRAYSCALEIMAGEPROCESSOR_H
#define GRAYSCALEIMAGEPROCESSOR_H
#include "ImageProcessor.h"

class GrayscaleImageProcessor : public ImageProcessor{
public:
    GrayscaleImageProcessor(Mat &image, int brightnessModVal, int contrastLinearModVal, float contrastGammaModVal);
    void modifyBrightness() override;
    void mofifyContrastLinear() override;
    void modifyContrastGamma() override;
    void negative() override;
    void flipHorizontally() override;
    void flipVertically() override;
    void flipDiagonally() override;
    Mat resize(float factor) override;
    Mat midpointFilter() override;
    Mat arithmeticMeanFilter() override;
};



#endif //GRAYSCALEIMAGEPROCESSOR_H
