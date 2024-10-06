//
// Created by gluckasz on 10/6/24.
//

#include "../include/GrayscaleImageProcessor.h"
GrayscaleImageProcessor::GrayscaleImageProcessor(Mat &image, int brightnessModVal, float contrastModVal) {
    this->image = image;
    this->brightnessModVal = brightnessModVal;
    this->contrastModVal = contrastModVal;
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

void GrayscaleImageProcessor::modifyContrast() {
    for (int x = 0; x < image.rows; x ++) {
        for (int y = 0; y < image.cols; y++) {
            float normalizedPixelVal = static_cast<float>(image.at<uchar>(x, y)) / 255.0f;

            // Apply gamma correction
            float gammaCorrectedVal = pow(normalizedPixelVal, contrastModVal); // Gamma correction
            int gammaCorrectedPixel = round(gammaCorrectedVal * 255); // Scale back to 0-255

            gammaCorrectedPixel = std::min(std::max(gammaCorrectedPixel, 0), 255);
            image.at<uchar>(x, y) = gammaCorrectedPixel;
        }
    }
}


