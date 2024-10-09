//
// Created by gluckasz on 10/7/24.
//

#include "../include/RGBImageProcessor.h"

cv::Mat RGBImageProcessor::modifyBrightness(cv::Mat image, int modVal) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (modVal < 0) {
                    if (image.at<cv::Vec3b>(x, y)[z] >= 0 - modVal) {
                        image.at<cv::Vec3b>(x, y)[z] += modVal;
                    }
                    else {
                        image.at<cv::Vec3b>(x, y)[z] = 0;
                    }
                }
                else {
                    if (image.at<cv::Vec3b>(x, y)[z]<= UCHAR_MAX - modVal) {
                        image.at<cv::Vec3b>(x, y)[z]+= modVal;
                    }
                    else {
                        image.at<cv::Vec3b>(x, y)[z] = UCHAR_MAX;
                    }
                }
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::mofifyContrastLinear(cv::Mat image, int modVal) {
    uchar max[3] = {0, 0, 0};
    uchar min[3] = {255, 255, 255};
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (max[z] < image.at<cv::Vec3b>(x, y)[z]) {
                    max[z] = image.at<cv::Vec3b>(x, y)[z];
                }
                if (min[z] > image.at<cv::Vec3b>(x, y)[z]) {
                    min[z] = image.at<cv::Vec3b>(x, y)[z];
                }
            }
        }
    }
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                image.at<cv::Vec3b>(x, y)[z] = std::clamp(
                    (image.at<cv::Vec3b>(x, y)[z] - min[z])
                    * (max[z] - min[z] + 2 * modVal)
                    / (max[z] - min[z]) + min[z],
                    0,
                    UCHAR_MAX
                    );
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::modifyContrastGamma(cv::Mat image, float modVal) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                float normalizedPixelVal = static_cast<float>(image.at<cv::Vec3b>(x, y)[z]) / 255.0f;

                // Apply gamma correction
                float gammaCorrectedVal = pow(normalizedPixelVal, modVal); // Gamma correction
                int gammaCorrectedPixel = round(gammaCorrectedVal * 255); // Scale back to 0-255

                gammaCorrectedPixel = std::clamp(gammaCorrectedPixel, 0, UCHAR_MAX);
                image.at<cv::Vec3b>(x, y)[z] = gammaCorrectedPixel;
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::negative(cv::Mat image) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                image.at<cv::Vec3b>(x, y)[z] = 255 - image.at<cv::Vec3b>(x, y)[z];
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::flipHorizontally(cv::Mat image) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols / 2; y++) {
            for (int z = 0; z < 3; z++) {
                uchar temp = image.at<cv::Vec3b>(x, y)[z];
                image.at<cv::Vec3b>(x, y)[z] = image.at<cv::Vec3b>(x, image.cols - y - 1)[z];
                image.at<cv::Vec3b>(x, image.cols - y - 1)[z] = temp;
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::flipVertically(cv::Mat image) {
    for (int x = 0; x < image.rows / 2; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                uchar temp = image.at<cv::Vec3b>(x, y)[z];
                image.at<cv::Vec3b>(x, y)[z] = image.at<cv::Vec3b>(image.rows - x - 1, y)[z];
                image.at<cv::Vec3b>(image.rows - x - 1, y)[z] = temp;
            }
        }
    }
    return image;
}
cv::Mat RGBImageProcessor::flipDiagonally(cv::Mat image) {
    image = this->flipHorizontally(image);
    return this->flipVertically(image);
}

cv::Mat RGBImageProcessor::resize(cv::Mat image, float factor) {
    int newWidth = static_cast<int>(static_cast<float>(image.cols) * factor);
    int newHeight = static_cast<int>(static_cast<float>(image.rows) * factor);
    cv::Mat  newImage = cv::Mat ::zeros(newHeight, newWidth, CV_8UC3);
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            for (int z = 0; z < 3; z++) {
            newImage.at<cv::Vec3b>(y, x)[z] = image.at<cv::Vec3b>(
                static_cast<int>(static_cast<float>(y) / factor),
                static_cast<int>(static_cast<float>(x) / factor))[z];
            }
        }
    }
    return newImage;
}

cv::Mat RGBImageProcessor::midpointFilter(cv::Mat image) {
    cv::Mat  newImage;
    image.copyTo(newImage);
    for (int y = 1; y < image.rows - 1; y++) {
        for (int x = 1; x < image.cols - 1; x++) {
            for (int z = 0; z < 2; z++) {
                uchar max = image.at<cv::Vec3b>(y - 1, x - 1)[z];
                uchar min = image.at<cv::Vec3b>(y - 1, x - 1)[z];
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (max < image.at<cv::Vec3b>(y - j, x - i)[z]) {
                            max = image.at<cv::Vec3b>(y - j, x - i)[z];
                        }
                        if (min > image.at<cv::Vec3b>(y - j, x - i)[z]) {
                            min = image.at<cv::Vec3b>(y - j, x - i)[z];
                        }
                    }
                }
                newImage.at<cv::Vec3b>(y, x)[z] = (max + min) / 2;
            }
        }
    }
    return newImage;
}

cv::Mat RGBImageProcessor::arithmeticMeanFilter(cv::Mat image) {
    cv::Mat newImage;
    image.copyTo(newImage);
    for (int y = 1; y < image.rows - 1; y++) {
        for (int x = 1; x < image.cols - 1; x++) {
            for (int z = 0; z < 2; z++) {
                int sum = 0;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        sum += image.at<cv::Vec3b>(y - j, x - i)[z];
                    }
                }
                newImage.at<cv::Vec3b>(y, x)[z] = sum / 9;
            }
        }
    }
    return newImage;
}
