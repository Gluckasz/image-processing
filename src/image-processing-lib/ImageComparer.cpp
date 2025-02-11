//
// Created by gluckasz on 2/1/25.
//

#include "../../include/image-processing-lib/ImageComparer.h"

namespace ImageComparer {
    double meanSquareError(cv::Mat originalImage, cv::Mat newImage) {
        if (originalImage.empty() || newImage.empty()) {
            throw std::invalid_argument("Images cannot be empty");
        }
        if (originalImage.cols != newImage.cols || originalImage.rows != newImage.rows) {
            throw std::invalid_argument("Images must have same dimensions");
        }
        double squareDistanceSum = 0;

        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                squareDistanceSum += pow(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y), 2);
            }
        }

        return squareDistanceSum
               / static_cast<double>(originalImage.cols)
               / static_cast<double>(originalImage.rows);
    }

    double peakMeanSquareError(cv::Mat originalImage, const cv::Mat &newImage) {
        if (originalImage.empty() || newImage.empty()) {
            throw std::invalid_argument("Images cannot be empty");
        }
        if (originalImage.cols != newImage.cols || originalImage.rows != newImage.rows) {
            throw std::invalid_argument("Images must have same dimensions");
        }
        uchar max = 0;

        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                if (originalImage.at<uchar>(x, y) > max) {
                    max = originalImage.at<uchar>(x, y);
                }
            }
        }
        if (max == 0) {
            throw std::invalid_argument("Image maximum value cannot be zero");
        }
        return meanSquareError(originalImage, newImage)
               / pow(max, 2);
    }

    double signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) {
        if (originalImage.empty() || newImage.empty()) {
            throw std::invalid_argument("Images cannot be empty");
        }
        if (originalImage.cols != newImage.cols || originalImage.rows != newImage.rows) {
            throw std::invalid_argument("Images must have same dimensions");
        }
        double squareSum = 0;
        double se = 0;

        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                squareSum += pow(originalImage.at<uchar>(x, y), 2);
                se += pow(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y), 2);
            }
        }
        if (se == 0) {
            throw std::invalid_argument("Squared error value cannot be zero");
        }
        if (squareSum == 0) {
            throw std::invalid_argument("Squared sum value of original image cannot be zero");
        }

        return 10 * std::log10(
                   squareSum
                   / se
               );
    }

    double peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) {
        if (originalImage.empty() || newImage.empty()) {
            throw std::invalid_argument("Images cannot be empty");
        }
        if (originalImage.cols != newImage.cols || originalImage.rows != newImage.rows) {
            throw std::invalid_argument("Images must have same dimensions");
        }
        uchar max = 0;
        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                if (originalImage.at<uchar>(x, y) > max) {
                    max = originalImage.at<uchar>(x, y);
                }
            }
        }
        if (max == 0) {
            throw std::invalid_argument("Image maximum value cannot be zero");
        }

        unsigned long long maxSquareSum = 0;
        double se = 0;
        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                maxSquareSum += static_cast<unsigned long long>(std::round(pow(static_cast<int>(max), 2)));
                se += pow(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y), 2);
            }
        }
        if (se == 0) {
            throw std::invalid_argument("Squared error value cannot be zero");
        }

        return 10 * std::log10(
                   static_cast<double>(maxSquareSum)
                   / se);
    }


    double maximumDifference(cv::Mat originalImage, cv::Mat newImage) {
        if (originalImage.empty() || newImage.empty()) {
            throw std::invalid_argument("Images cannot be empty");
        }
        if (originalImage.cols != newImage.cols || originalImage.rows != newImage.rows) {
            throw std::invalid_argument("Images must have same dimensions");
        }
        uchar maxDifference = 0;
        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                if (std::abs(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y)) > maxDifference) {
                    maxDifference = std::abs(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y));
                }
            }
        }
        return maxDifference;
    }
}

