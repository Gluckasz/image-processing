//
// Created by gluckasz on 10/7/24.
//

#ifndef RGBIMAGEPROCESSOR_H
#define RGBIMAGEPROCESSOR_H
#include "GrayscaleImageProcessor.h"


class RGBImageProcessor : public ImageProcessor {
public:
    /**
     * Modify brightness of a RGB image by a constant factor.
     * @param image image to modify.
     * @param modVal brightness modification value (positive makes image brighter and negative makes image darker).
     * @return modified image.
     */
    cv::Mat modifyBrightness(cv::Mat image, int modVal) override;
    cv::Mat mofifyContrastLinear(cv::Mat image, int modVal) override;
    cv::Mat modifyContrastGamma(cv::Mat image, float modVal) override;
    cv::Mat negative(cv::Mat image) override;
    cv::Mat flipHorizontally(cv::Mat image) override;
    cv::Mat flipVertically(cv::Mat image) override;
    cv::Mat flipDiagonally(cv::Mat image) override;
    cv::Mat resize(cv::Mat image, float factor) override;
    cv::Mat midpointFilter(cv::Mat image) override;
    cv::Mat arithmeticMeanFilter(cv::Mat image) override;
    std::string meanSquareError(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) override;
    std::string peakMeanSquareError(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) override;
    std::string signalToNoiseRatio(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) override;
    std::string peakSignalToNoiseRatio(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) override;
    std::string maximumDifference(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) override;
};



#endif //RGBIMAGEPROCESSOR_H
