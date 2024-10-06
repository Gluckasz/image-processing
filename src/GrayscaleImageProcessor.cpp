//
// Created by gluckasz on 10/6/24.
//

#include "../include/GrayscaleImageProcessor.h"
GrayscaleImageProcessor::GrayscaleImageProcessor(Mat &image, bool isBrightnessModified, int brightnessModVal) {
    this->image = image;
    this->isBrightnessModified = isBrightnessModified;
    this->brightnessModVal = brightnessModVal;
}

void GrayscaleImageProcessor::modifyBrightness() {
    for (int x = 0; x < image.rows; x ++) {
        for (int y = 0; y < image.cols; y++) {
            if (brightnessModVal < 0) {
                if (image.at<uchar>(x, y) >= 0 - brightnessModVal) {
                    image.at<uchar>(x, y) += brightnessModVal;
                }
                else {
                    image.at<uchar>(x, y) = 0;
                }
            }
            else {
                if (image.at<uchar>(x, y) <= UCHAR_MAX - brightnessModVal) {
                    image.at<uchar>(x, y) += brightnessModVal;
                }
                else {
                    image.at<uchar>(x, y) = UCHAR_MAX;
                }
            }
        }
    }
}

