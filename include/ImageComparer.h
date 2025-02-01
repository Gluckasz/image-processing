//
// Created by gluckasz on 2/1/25.
//

#ifndef IMAGECOMPARER_H
#define IMAGECOMPARER_H
#include <opencv2/opencv.hpp>


class ImageComparer {
public:
    static double meanSquareError(cv::Mat originalImage, cv::Mat newImage);

    static double peakMeanSquareError(cv::Mat originalImage, const cv::Mat &newImage);

    static double signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage);

    static double peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage);

    static double maximumDifference(cv::Mat originalImage, cv::Mat newImage);
};


#endif //IMAGECOMPARER_H
