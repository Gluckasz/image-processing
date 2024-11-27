//
// Created by gluckasz on 10/6/24.
//

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>

class ImageProcessor {
public:
    virtual ~ImageProcessor() = default;

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
    virtual std::array<uint, UCHAR_MAX + 1> computeHistogram(cv::Mat image, int histogramChanngel, uint& histogramMaxVal) = 0;
    virtual cv::Mat histogram(cv::Mat image, int histogramChannel) = 0;
    virtual cv::Mat histogramUniform(cv::Mat image, int gMax, int gMin) = 0;
    virtual double mean(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double variance(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double standardDeviation(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double variation1(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double asymmetry(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double flattening(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double variation2(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double entropy(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual cv::Mat laplacianFilter(cv::Mat image, int laplaceMask) = 0;
    virtual cv::Mat optimizedLaplacianFilter(cv::Mat image) = 0;
    virtual cv::Mat robertsOperator1(cv::Mat image) = 0;
};

#endif //IMAGEPROCESSOR_H
