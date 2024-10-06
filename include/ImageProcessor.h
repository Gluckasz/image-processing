//
// Created by gluckasz on 10/6/24.
//

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
using namespace cv;

class ImageProcessor {
public:
    virtual void modifyBrightness() = 0;
    virtual void modifyContrast() = 0;
protected:
    Mat image;
    int brightnessModVal;
    float contrastModVal;
};

#endif //IMAGEPROCESSOR_H
