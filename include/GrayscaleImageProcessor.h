//
// Created by gluckasz on 10/6/24.
//

#ifndef GRAYSCALEIMAGEPROCESSOR_H
#define GRAYSCALEIMAGEPROCESSOR_H
#include "SpatialDomainProcessor.h"

class GrayscaleImageProcessor final : public SpatialDomainProcessor {
public:
    /**
     * Modify brightness of a grayscale image by a constant factor.
     * @param image image to modify
     * @param modVal brightness modification value (positive makes image brighter and negative makes image darker)
     * @return image with modified brightness
     */
    cv::Mat modifyBrightness(cv::Mat image, int modVal) override;

    /**
     * Modify contrast of a grayscale image using linear contrast stretching.
     * @param image image to modify
     * @param modVal contrast stretch modification value (positive makes contrast larger
     * and negative makes contrast smaller)
     * @return image with modified contrast
     */
    cv::Mat modifyContrastLinear(cv::Mat image, int modVal) override;

    /**
     * Modify contrast of a grayscale image using gamma contrast correction.
     * @param image image to modify
     * @param modVal floating-point value indicating value of gamma
     * @return image with modified contrast
     */
    cv::Mat modifyContrastGamma(cv::Mat image, float modVal) override;

    /**
     * Take the negative of a grayscale image.
     * @param image image to take the negative of
     * @return negative of an image
     */
    cv::Mat negative(cv::Mat image) override;

    /**
     * Flip the image horizontally.
     * @param image to be flipped
     * @return flipped image horizontally
     */
    cv::Mat flipHorizontally(cv::Mat image) override;

    /**
     * Flip the image vertically.
     * @param image to be flipped
     * @return flipped image vertically
     */
    cv::Mat flipVertically(cv::Mat image) override;

    /**
     * Flip the image diagonally.
     * @param image to be flipped
     * @return flipped image diagonally
     */
    cv::Mat flipDiagonally(cv::Mat image) override;

    /**
     * Resize an image by a factor.
     * @param image to be resized
     * @param factor of the resize operation
     * @return image of size = oldImage * factor
     */
    cv::Mat resize(cv::Mat image, float factor) override;

    /**
     * Apply a midpoint filter.
     * @param image to apply filter to
     * @param kernelSize of the filter
     * @return image after applying the filter
     */
    cv::Mat midpointFilter(cv::Mat image, int kernelSize) override;

    /**
     * Apply an arithmetic filter.
     * @param image to apply filter to
     * @param kernelSize of the filter
     * @return image after applying the filter
     */
    cv::Mat arithmeticMeanFilter(cv::Mat image, int kernelSize) override;


    double meanSquareError(cv::Mat originalImage, cv::Mat newImage) override;


    double peakMeanSquareError(cv::Mat originalImage, cv::Mat newImage) override;


    double signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) override;


    double peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) override;


    double maximumDifference(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Apply Laplacian edge detection filter.
     * @param image Input image
     * @param laplaceMask Type of Laplacian mask to use
     * @return Edge detected image
     */
    cv::Mat laplacianFilter(cv::Mat image, int laplaceMask) override;

    /**
     * Apply optimized Laplacian filter for edge detection.
     * @param image Input image
     * @return Edge detected image
     */
    cv::Mat optimizedLaplacianFilter(cv::Mat image) override;

    /**
     * Apply Roberts cross gradient operator for edge detection.
     * @param image Input image
     * @return Edge detected image
     */
    cv::Mat robertsOperator1(cv::Mat image) override;
};


#endif //GRAYSCALEIMAGEPROCESSOR_H
