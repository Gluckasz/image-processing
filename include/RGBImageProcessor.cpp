//
// Created by gluckasz on 10/7/24.
//

#include "RGBImageProcessor.h"

RGBImageProcessor::RGBImageProcessor(Mat &image, int brightnessModVal, int contrastLinearModVal, float contrastGammaModVal) {
    this->image = image;
    this->brightnessModVal = brightnessModVal;
    this->contrastLinearModVal = contrastLinearModVal;
    this->contrastGammaModVal = contrastGammaModVal;
}

void RGBImageProcessor::modifyBrightness() {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (brightnessModVal < 0) {
                    if (image.at<Vec3b>(x, y)[z] >= 0 - brightnessModVal) {
                        image.at<Vec3b>(x, y)[z] += brightnessModVal;
                    }
                    else {
                        image.at<Vec3b>(x, y)[z] = 0;
                    }
                }
                else {
                    if (image.at<Vec3b>(x, y)[z]<= UCHAR_MAX - brightnessModVal) {
                        image.at<Vec3b>(x, y)[z]+= brightnessModVal;
                    }
                    else {
                        image.at<Vec3b>(x, y)[z] = UCHAR_MAX;
                    }
                }
            }
        }
    }
}

void RGBImageProcessor::mofifyContrastLinear() {
    uchar max[3] = {0, 0, 0};
    uchar min[3] = {255, 255, 255};
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (max[z] < image.at<Vec3b>(x, y)[z]) {
                    max[z] = image.at<Vec3b>(x, y)[z];
                }
                if (min[z] > image.at<Vec3b>(x, y)[z]) {
                    min[z] = image.at<Vec3b>(x, y)[z];
                }
            }
        }
    }
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                image.at<Vec3b>(x, y)[z] = std::clamp(
                    (image.at<Vec3b>(x, y)[z] - min[z])
                    * (max[z] - min[z] + 2 * contrastLinearModVal)
                    / (max[z] - min[z]) + min[z],
                    0,
                    UCHAR_MAX
                    );
            }
        }
    }
}

void RGBImageProcessor::modifyContrastGamma() {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                float normalizedPixelVal = static_cast<float>(image.at<Vec3b>(x, y)[z]) / 255.0f;

                // Apply gamma correction
                float gammaCorrectedVal = pow(normalizedPixelVal, contrastGammaModVal); // Gamma correction
                int gammaCorrectedPixel = round(gammaCorrectedVal * 255); // Scale back to 0-255

                gammaCorrectedPixel = std::clamp(gammaCorrectedPixel, 0, UCHAR_MAX);
                image.at<Vec3b>(x, y)[z] = gammaCorrectedPixel;
            }
        }
    }
}

void RGBImageProcessor::negative() {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                image.at<Vec3b>(x, y)[z] = 255 - image.at<Vec3b>(x, y)[z];
            }
        }
    }
}

void RGBImageProcessor::flipHorizontally() {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols / 2; y++) {
            for (int z = 0; z < 3; z++) {
                uchar temp = image.at<Vec3b>(x, y)[z];
                image.at<Vec3b>(x, y)[z] = image.at<Vec3b>(x, image.cols - y - 1)[z];
                image.at<Vec3b>(x, image.cols - y - 1)[z] = temp;
            }
        }
    }
}

void RGBImageProcessor::flipVertically() {
    for (int x = 0; x < image.rows / 2; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                uchar temp = image.at<Vec3b>(x, y)[z];
                image.at<Vec3b>(x, y)[z] = image.at<Vec3b>(image.rows - x - 1, y)[z];
                image.at<Vec3b>(image.rows - x - 1, y)[z] = temp;
            }
        }
    }
}
void RGBImageProcessor::flipDiagonally() {
    this->flipHorizontally();
    this->flipVertically();
}

Mat RGBImageProcessor::resize(float factor) {
    int newWidth = static_cast<int>(static_cast<float>(image.cols) * factor);
    int newHeight = static_cast<int>(static_cast<float>(image.rows) * factor);
    Mat newImage = Mat::zeros(newHeight, newWidth, CV_8UC3);
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            for (int z = 0; z < 3; z++) {
            newImage.at<Vec3b>(y, x)[z] = image.at<Vec3b>(
                static_cast<int>(static_cast<float>(y) / factor),
                static_cast<int>(static_cast<float>(x) / factor))[z];
            }
        }
    }
    return newImage;
}




