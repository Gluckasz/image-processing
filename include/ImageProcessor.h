//
// Created by gluckasz on 10/6/24.
//

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
using namespace cv;

class ImageProcessor {
public:
    virtual ~ImageProcessor() = default;

    virtual void modifyBrightness() = 0;
    virtual void mofifyContrastLinear() = 0;
    virtual void modifyContrastGamma() = 0;
    virtual void negative() = 0;
    virtual void flipHorizontally() = 0;
    virtual void flipVertically() = 0;
    virtual void flipDiagonally() = 0;
protected:
    Mat image;
    int brightnessModVal;
    int contrastLinearModVal;
    float contrastGammaModVal;
};

#endif //IMAGEPROCESSOR_H
