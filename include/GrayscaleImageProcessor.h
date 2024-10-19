//
// Created by gluckasz on 10/6/24.
//

#ifndef GRAYSCALEIMAGEPROCESSOR_H
#define GRAYSCALEIMAGEPROCESSOR_H
#include "ImageProcessor.h"

class GrayscaleImageProcessor : public ImageProcessor {
public:
    /**
     * Modify brightness of a grayscale image by a constant factor.
     * @param image image to modify.
     * @param modVal brightness modification value (positive makes image brighter and negative makes image darker).
     * @return image with modified brightness.
     */
    cv::Mat modifyBrightness(cv::Mat image, int modVal) override;

    /**
     * Modify contrast of a grayscale image using linear contrast stretching.
     * @param image image to modify.
     * @param modVal contrast stretch modification value (positive makes contrast larger
     * and negative makes contrast smaller).
     * @return image with modified contrast.
     */
    cv::Mat mofifyContrastLinear(cv::Mat image, int modVal) override;

    /**
     * Modify contrast of a grayscale image using gamma contrast correction.
     * @param image image to modify.
     * @param modVal floating-point value indicating value of gamma.
     * @return image with modified contrast.
     */
    cv::Mat modifyContrastGamma(cv::Mat image, float modVal) override;

    /**
     * Take the negative of a grayscale image.
     * @param image image to take the negative of.
     * @return negative of an image.
     */
    cv::Mat negative(cv::Mat image) override;

    /**
     * Flip the image horizontally.
     * @param image to be flipped.
     * @return flipped image horizontally.
     */
    cv::Mat flipHorizontally(cv::Mat image) override;

    /**
     * Flip the image vertically.
     * @param image to be flipped.
     * @return flipped image vertically.
     */
    cv::Mat flipVertically(cv::Mat image) override;

    /**
     * Flip the image diagonally.
     * @param image to be flipped.
     * @return flipped image diagonally.
     */
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



#endif //GRAYSCALEIMAGEPROCESSOR_H
