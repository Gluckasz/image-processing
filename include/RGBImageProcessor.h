//
// Created by gluckasz on 10/7/24.
//

#ifndef RGBIMAGEPROCESSOR_H
#define RGBIMAGEPROCESSOR_H
#include "GrayscaleImageProcessor.h"


class RGBImageProcessor final : public ImageProcessor {
public:
    /**
     * Modify brightness of an RGB image by a constant factor.
     * @param image image to modify.
     * @param modVal brightness modification value (positive makes image brighter and negative makes image darker).
     * @return image with modified brightness.
     */
    cv::Mat modifyBrightness(cv::Mat image, int modVal) override;

    /**
     * Modify contrast of an RGB image using linear contrast stretching.
     * @param image image to modify.
     * @param modVal contrast stretch modification value (positive makes contrast larger
     * and negative makes contrast smaller).
     * @return image with modified contrast.
     */
    cv::Mat modifyContrastLinear(cv::Mat image, int modVal) override;

    /**
     * Modify contrast of an RGB image using gamma contrast correction.
     * @param image image to modify.
     * @param modVal floating-point value indicating value of gamma.
     * @return image with modified contrast.
     */
    cv::Mat modifyContrastGamma(cv::Mat image, float modVal) override;

    /**
     * Take the negative of an RGB image.
     * @param image image to take the negative of.
     * @return negative of an image
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

    /**
     * Resize an image by a factor.
     * @param image to be resized.
     * @param factor of the resize operation.
     * @return image of size = oldImage * factor.
     */
    cv::Mat resize(cv::Mat image, float factor) override;

    /**
     * Apply a midpoint filter.
     * @param image to apply filter to.
     * @param kernelSize of the filter.
     * @return image after applying the filter.
     */
    cv::Mat midpointFilter(cv::Mat image, int kernelSize) override;

    /**
     * Apply an arithmetic filter.
     * @param image to apply filter to.
     * @param kernelSize of the filter.
     * @return image after applying the filter.
     */
    cv::Mat arithmeticMeanFilter(cv::Mat image, int kernelSize) override; /**
     * Compute the mean square error between two RGB images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Mean square error value
     */
    double meanSquareError(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Compute peak mean square error between two RGB images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Peak mean square error value
     */
    double peakMeanSquareError(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Calculate signal-to-noise ratio between original and processed RGB images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Signal-to-noise ratio value
     */
    double signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Calculate peak signal-to-noise ratio between original and processed RGB images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Peak signal-to-noise ratio value in decibels
     */
    double peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Find maximum pixel difference between two RGB images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Maximum absolute difference between corresponding pixels
     */
    double maximumDifference(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Compute histogram of the RGB image for specified channel.
     * @param image Input RGB image
     * @param histogramChannel Channel to compute histogram for (0=B, 1=G, 2=R)
     * @param histogramMaxVal Output parameter for maximum histogram value
     * @return Array containing histogram values
     */
    std::array<uint, UCHAR_MAX + 1>
    computeHistogram(cv::Mat image, int histogramChannel, uint &histogramMaxVal) override;

    /**
     * Generate histogram visualization for RGB image channel.
     * @param image Input RGB image
     * @param histogramChannel Channel to visualize histogram for (0=B, 1=G, 2=R)
     * @return Image containing histogram visualization
     */
    cv::Mat histogram(cv::Mat image, int histogramChannel) override;

    /**
     * Perform histogram uniform transformation on RGB image.
     * @param image Input RGB image to transform
     * @param gMax Maximum output intensity
     * @param gMin Minimum output intensity
     * @return Transformed RGB image with uniform histogram
     */
    cv::Mat histogramUniform(cv::Mat image, int gMax, int gMin) override;

    /**
     * Calculate mean value from histogram of RGB channel.
     * @param imageHistogram Input histogram array
     * @return Mean value
     */
    double mean(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate variance from histogram of RGB channel.
     * @param imageHistogram Input histogram array
     * @return Variance value
     */
    double variance(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate standard deviation from histogram of RGB channel.
     * @param imageHistogram Input histogram array
     * @return Standard deviation value
     */
    double standardDeviation(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate first variation coefficient from histogram of RGB channel.
     * @param imageHistogram Input histogram array
     * @return First variation coefficient
     */
    double variation1(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate asymmetry coefficient from histogram of RGB channel.
     * @param imageHistogram Input histogram array
     * @return Asymmetry coefficient
     */
    double asymmetry(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate flattening coefficient from histogram of RGB channel.
     * @param imageHistogram Input histogram array
     * @return Flattening coefficient
     */
    double flattening(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate second variation coefficient from histogram of RGB channel.
     * @param imageHistogram Input histogram array
     * @return Second variation coefficient
     */
    double variation2(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate entropy from histogram of RGB channel.
     * @param imageHistogram Input histogram array
     * @return Entropy value
     */
    double entropy(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Apply Laplacian edge detection filter to RGB image.
     * @param image Input RGB image
     * @param laplaceMask Type of Laplacian mask to use
     * @return Edge detected RGB image
     */
    cv::Mat laplacianFilter(cv::Mat image, int laplaceMask) override;

    /**
     * Apply optimized Laplacian filter for edge detection on RGB image.
     * @param image Input RGB image
     * @return Edge detected RGB image
     */
    cv::Mat optimizedLaplacianFilter(cv::Mat image) override;

    /**
     * Apply Roberts cross gradient operator for edge detection on RGB image.
     * @param image Input RGB image
     * @return Edge detected RGB image
     */
    cv::Mat robertsOperator1(cv::Mat image) override;

    /**
     * Perform region growing segmentation on RGB image.
     * @param image Input RGB image
     * @param criterion Region growing criterion value
     * @return Segmented RGB image
     */
    cv::Mat regionGrowing(cv::Mat image, int criterion) override;

    /**
     * Compute inverse Fourier transform for RGB image.
     * @param fourierImages Vector of Fourier transformed RGB image components
     * @return Reconstructed spatial domain RGB image
     */
    cv::Mat inverseFourierTransform(std::vector<cv::Mat> fourierImages) override;

    /**
     * Compute inverse Fast Fourier transform for RGB image.
     * @param fourierImages Vector of Fourier transformed RGB image components
     * @return Reconstructed spatial domain RGB image
     */
    cv::Mat inverseFastFourierTransform(std::vector<cv::Mat> fourierImages) override;

};


#endif //RGBIMAGEPROCESSOR_H
