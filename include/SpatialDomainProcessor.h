//
// Created by gluckasz on 10/6/24.
//

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <thread>
#include <unordered_set>
#include <complex>

class SpatialDomainProcessor {
public:
    virtual ~SpatialDomainProcessor() = default;

    virtual cv::Mat modifyBrightness(cv::Mat image, int modVal) = 0;

    virtual cv::Mat modifyContrastLinear(cv::Mat image, int modVal) = 0;

    virtual cv::Mat modifyContrastGamma(cv::Mat image, float modVal) = 0;

    virtual cv::Mat negative(cv::Mat image) = 0;

    virtual cv::Mat flipHorizontally(cv::Mat image) = 0;

    virtual cv::Mat flipVertically(cv::Mat image) = 0;

    virtual cv::Mat flipDiagonally(cv::Mat image) = 0;

    virtual cv::Mat resize(cv::Mat image, float factor) = 0;

    virtual cv::Mat midpointFilter(cv::Mat image, int kernelSize) = 0;

    virtual cv::Mat arithmeticMeanFilter(cv::Mat image, int kernelSize) = 0;

    virtual double meanSquareError(cv::Mat originalImage, cv::Mat newImage) = 0;

    virtual double peakMeanSquareError(cv::Mat originalImage, cv::Mat newImage) = 0;

    virtual double signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) = 0;

    virtual double peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) = 0;

    virtual double maximumDifference(cv::Mat originalImage, cv::Mat newImage) = 0;

    virtual cv::Mat laplacianFilter(cv::Mat image, int laplaceMask) = 0;

    virtual cv::Mat optimizedLaplacianFilter(cv::Mat image) = 0;

    virtual cv::Mat robertsOperator1(cv::Mat image) = 0;

    /**
     * Perform region growing segmentation.
     * @param image Input image
     * @param criterion Region growing criterion value
     * @return Segmented image
     */
    cv::Mat regionGrowing(cv::Mat image, int criterion);
};

#endif //IMAGEPROCESSOR_H
