//
// Created by gluckasz on 2/1/25.
//

#include "../../include/image-processing-lib/ImageComparer.h"

namespace ImageComparer {
    double meanSquareError(cv::Mat originalImage, cv::Mat newImage) {
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
        uchar max = 0;

        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                if (originalImage.at<uchar>(x, y) > max) {
                    max = originalImage.at<uchar>(x, y);
                }
            }
        }
        return meanSquareError(originalImage, newImage)
               / pow(max, 2);
    }

    double signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) {
        double squareSum = 0;
        double se = 0;

        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                squareSum += pow(originalImage.at<uchar>(x, y), 2);
                se += pow(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y), 2);
            }
        }

        return 10 * std::log10(
                   squareSum
                   / se
               );
    }

    double peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) {
        uchar max = 0;
        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                if (originalImage.at<uchar>(x, y) > max) {
                    max = originalImage.at<uchar>(x, y);
                }
            }
        }

        unsigned long long maxSquareSum = 0;
        double se = 0;
        for (int x = 0; x < originalImage.cols; x++) {
            for (int y = 0; y < originalImage.rows; y++) {
                maxSquareSum += static_cast<unsigned long long>(std::round(pow(static_cast<int>(max), 2)));
                se += pow(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y), 2);
            }
        }

        return 10 * std::log10(
                   static_cast<double>(maxSquareSum)
                   / se);
    }


    double maximumDifference(cv::Mat originalImage, cv::Mat newImage) {
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

