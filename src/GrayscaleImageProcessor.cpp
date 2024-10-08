//
// Created by gluckasz on 10/6/24.
//

#include "../include/GrayscaleImageProcessor.h"
GrayscaleImageProcessor::GrayscaleImageProcessor(Mat &image, int brightnessModVal, int contrastLinearModVal, float contrastGammaModVal) {
    this->image = image;
    this->brightnessModVal = brightnessModVal;
    this->contrastLinearModVal = contrastLinearModVal;
    this->contrastGammaModVal = contrastGammaModVal;
}

void GrayscaleImageProcessor::modifyBrightness() {
    for (int x = 0; x < image.rows; x++) {
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

void GrayscaleImageProcessor::mofifyContrastLinear() {
    uchar max = 0;
    uchar min = 255;
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            if (max < image.at<uchar>(x, y)) {
                max = image.at<uchar>(x, y);
            }
            if (min > image.at<uchar>(x, y)) {
                min = image.at<uchar>(x, y);
            }
        }
    }
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            image.at<uchar>(x, y) = std::clamp(
                (image.at<uchar>(x, y) - min)
                * (max - min + 2 * contrastLinearModVal)
                / (max - min) + min,
                0,
                UCHAR_MAX
                );
        }
    }
}


void GrayscaleImageProcessor::modifyContrastGamma() {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            float normalizedPixelVal = static_cast<float>(image.at<uchar>(x, y)) / 255.0f;

            // Apply gamma correction
            float gammaCorrectedVal = pow(normalizedPixelVal, contrastGammaModVal); // Gamma correction
            int gammaCorrectedPixel = round(gammaCorrectedVal * 255); // Scale back to 0-255

            gammaCorrectedPixel = std::clamp(gammaCorrectedPixel, 0, UCHAR_MAX);
            image.at<uchar>(x, y) = gammaCorrectedPixel;
        }
    }
}

void GrayscaleImageProcessor::negative() {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            image.at<uchar>(x, y) = 255 - image.at<uchar>(x, y);
        }
    }
}

void GrayscaleImageProcessor::flipHorizontally() {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols / 2; y++) {
            uchar temp = image.at<uchar>(x, y);
            image.at<uchar>(x, y) = image.at<uchar>(x, image.cols - y - 1);
            image.at<uchar>(x, image.cols - y - 1) = temp;
        }
    }
}

void GrayscaleImageProcessor::flipVertically() {
    for (int x = 0; x < image.rows / 2; x++) {
        for (int y = 0; y < image.cols; y++) {
            uchar temp = image.at<uchar>(x, y);
            image.at<uchar>(x, y) = image.at<uchar>(image.rows - x - 1, y);
            image.at<uchar>(image.rows - x - 1, y)= temp;
        }
    }
}




