//
// Created by gluckasz on 2/1/25.
//

#ifndef IMAGECOMPARER_H
#define IMAGECOMPARER_H
#include <opencv2/opencv.hpp>


class ImageComparer {
public:
    /**
     * Compute the mean square error between two images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Mean square error value
     */
    static double meanSquareError(cv::Mat originalImage, cv::Mat newImage);

    /**
     * Compute the peak mean square error between two images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Peak mean square error value
     */
    static double peakMeanSquareError(cv::Mat originalImage, const cv::Mat &newImage);

    /**
     * Calculate a signal-to-noise ratio between original and processed image.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Signal-to-noise ratio value
     */
    static double signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage);

    /**
     * Calculate a peak signal-to-noise ratio between original and processed image.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Peak signal-to-noise ratio value in decibels
     */
    static double peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage);

    /**
     * Find the maximum pixel difference between two images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Maximum absolute difference between corresponding pixels
     */
    static double maximumDifference(cv::Mat originalImage, cv::Mat newImage);
};


#endif //IMAGECOMPARER_H
