//
// Created by gluckasz on 10/6/24.
//

#include "../include/GrayscaleImageProcessor.h"

cv::Mat GrayscaleImageProcessor::modifyBrightness(cv::Mat image, int modVal){
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            if (modVal < 0) {
                if (image.at<uchar>(x, y) >= 0 - modVal) {
                    image.at<uchar>(x, y) += modVal;
                }
                else {
                    image.at<uchar>(x, y) = 0;
                }
            }
            else {
                if (image.at<uchar>(x, y) <= UCHAR_MAX - modVal) {
                    image.at<uchar>(x, y) += modVal;
                }
                else {
                    image.at<uchar>(x, y) = UCHAR_MAX;
                }
            }
        }
    }
    return image;
}

cv::Mat GrayscaleImageProcessor::mofifyContrastLinear(cv::Mat image, int modVal) {
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
                * (max - min + 2 * modVal)
                / (max - min) + min,
                0,
                UCHAR_MAX
                );
        }
    }
    return image;
}


cv::Mat GrayscaleImageProcessor::modifyContrastGamma(cv::Mat image, float modVal){
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            float normalizedPixelVal = static_cast<float>(image.at<uchar>(x, y)) / 255.0f;

            // Apply gamma correction
            float gammaCorrectedVal = pow(normalizedPixelVal, modVal); // Gamma correction
            int gammaCorrectedPixel = round(gammaCorrectedVal * 255); // Scale back to 0-255

            gammaCorrectedPixel = std::clamp(gammaCorrectedPixel, 0, UCHAR_MAX);
            image.at<uchar>(x, y) = gammaCorrectedPixel;
        }
    }
    return image;
}

cv::Mat GrayscaleImageProcessor::negative(cv::Mat image) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            image.at<uchar>(x, y) = 255 - image.at<uchar>(x, y);
        }
    }
    return image;
}

cv::Mat GrayscaleImageProcessor::flipHorizontally(cv::Mat image) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols / 2; y++) {
            uchar temp = image.at<uchar>(x, y);
            image.at<uchar>(x, y) = image.at<uchar>(x, image.cols - y - 1);
            image.at<uchar>(x, image.cols - y - 1) = temp;
        }
    }
    return image;
}

cv::Mat GrayscaleImageProcessor::flipVertically(cv::Mat image) {
    for (int x = 0; x < image.rows / 2; x++) {
        for (int y = 0; y < image.cols; y++) {
            uchar temp = image.at<uchar>(x, y);
            image.at<uchar>(x, y) = image.at<uchar>(image.rows - x - 1, y);
            image.at<uchar>(image.rows - x - 1, y)= temp;
        }
    }
    return image;
}

cv::Mat GrayscaleImageProcessor::flipDiagonally(cv::Mat image) {
    image = this->flipHorizontally(image);
    return this->flipVertically(image);
}

cv::Mat GrayscaleImageProcessor::resize(cv::Mat image, float factor) {
    int newWidth = static_cast<int>(static_cast<float>(image.cols) * factor);
    int newHeight = static_cast<int>(static_cast<float>(image.rows) * factor);
    cv::Mat newImage = cv::Mat::zeros(newHeight, newWidth, CV_8UC1);

    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            newImage.at<uchar>(y, x) = image.at<uchar>(
                static_cast<int>(static_cast<float>(y) / factor),
                static_cast<int>(static_cast<float>(x) / factor));
        }
    }
    return newImage;
}

cv::Mat GrayscaleImageProcessor::midpointFilter(cv::Mat image) {
    cv::Mat newImage;
    image.copyTo(newImage);
    for (int y = 1; y < image.rows - 1; y++) {
        for (int x = 1; x < image.cols - 1; x++) {
            uchar max = image.at<uchar>(y - 1, x - 1);
            uchar min = image.at<uchar>(y - 1, x - 1);
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (max < image.at<uchar>(y - j, x - i)) {
                        max = image.at<uchar>(y - j, x - i);
                    }
                    if (min > image.at<uchar>(y - j, x - i)) {
                        min = image.at<uchar>(y - j, x - i);
                    }
                }
            }
            newImage.at<uchar>(y, x) = (max + min) / 2;
        }
    }
    return newImage;
}

cv::Mat GrayscaleImageProcessor::arithmeticMeanFilter(cv::Mat image) {
    cv::Mat newImage;
    image.copyTo(newImage);
    for (int y = 1; y < image.rows - 1; y++) {
        for (int x = 1; x < image.cols - 1; x++) {
            int sum = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sum += image.at<uchar>(y - j, x - i);
                }
            }
            newImage.at<uchar>(y, x) = sum / 9;
        }
    }
    return newImage;
}

float computeMSE(cv::Mat compareImage, cv::Mat secondImage) {
    int squareDistanceSum = 0;

    for (int y = 0; y < compareImage.cols; y++) {
        for (int x = 0; x < compareImage.rows; x++) {
            squareDistanceSum += pow(compareImage.at<uchar>(y, x) - secondImage.at<uchar>(y, x) , 2);
        }
    }

    return static_cast<float>(squareDistanceSum)
    / static_cast<float>(compareImage.cols)
    / static_cast<float>(compareImage.rows);
}

std::string GrayscaleImageProcessor::meanSquareError(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }
    float mseBefore = computeMSE(compareImage, originalImage);

    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }

    float mseAfter= computeMSE(compareImage, newImage);

    std::stringstream ss;
    ss << "Mean square error before denoising: " << mseBefore << "\n"
    << "Mean square error after denoising: " << mseAfter << "\n";

    return ss.str();
}

float computeMSEAndSetMax(cv::Mat compareImage, cv::Mat secondImage, uchar &max) {
    int squareDistanceSum = 0;

    for (int y = 0; y < compareImage.cols; y++) {
        for (int x = 0; x < compareImage.rows; x++) {
            squareDistanceSum += pow(compareImage.at<uchar>(y, x) - secondImage.at<uchar>(y, x) , 2);
            if (compareImage.at<uchar>(y, x) > max) {
                max = compareImage.at<uchar>(y, x);
            }
        }
    }
    return static_cast<float>(squareDistanceSum)
    / static_cast<float>(compareImage.cols)
    / static_cast<float>(compareImage.rows);
}

std::string GrayscaleImageProcessor::peakMeanSquareError(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    uchar max = 0;

    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }
    float pmseBefore = computeMSEAndSetMax(compareImage, originalImage, max)
    / static_cast<float>(pow(max, 2));


    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }
    float pmseAfter= computeMSE(compareImage, newImage)
    / static_cast<float>(pow(max, 2));

    std::stringstream ss;
    ss << "Peak mean square error before denoising: " << pmseBefore << "\n"
    << "Peak mean square error after denoising: " << pmseAfter << "\n";

    return ss.str();
}


