//
// Created by gluckasz on 10/6/24.
//

#ifndef GRAYSCALEIMAGEPROCESSOR_H
#define GRAYSCALEIMAGEPROCESSOR_H
#include "ImageProcessor.h"

class GrayscaleImageProcessor : public ImageProcessor{
public:
    GrayscaleImageProcessor(Mat &image, bool isBrightnessModified, int brightnessModVal);
    void modifyBrightness() override;
};



#endif //GRAYSCALEIMAGEPROCESSOR_H
