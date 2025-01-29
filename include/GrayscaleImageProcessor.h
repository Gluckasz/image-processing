//
// Created by gluckasz on 10/6/24.
//

#ifndef GRAYSCALEIMAGEPROCESSOR_H
#define GRAYSCALEIMAGEPROCESSOR_H
#include "ImageProcessor.h"

class GrayscaleImageProcessor final : public ImageProcessor {
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

    /**
     * Compute the mean square error between two images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Mean square error value
     */
    double meanSquareError(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Compute the peak mean square error between two images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Peak mean square error value
     */
    double peakMeanSquareError(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Calculate signal-to-noise ratio between original and processed image.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Signal-to-noise ratio value
     */
    double signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Calculate peak signal-to-noise ratio between original and processed image.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Peak signal-to-noise ratio value in decibels
     */
    double peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Find the maximum pixel difference between two images.
     * @param originalImage Original image for comparison
     * @param newImage Modified/processed image
     * @return Maximum absolute difference between corresponding pixels
     */
    double maximumDifference(cv::Mat originalImage, cv::Mat newImage) override;

    /**
     * Compute histogram of the image for specified channel.
     * @param image Input image
     * @param histogramChannel Channel to compute histogram for (0 for grayscale)
     * @param histogramMaxVal Output parameter for maximum histogram value
     * @return Array containing histogram values
     */
    std::array<uint, UCHAR_MAX + 1>
    computeHistogram(cv::Mat image, int histogramChannel, uint &histogramMaxVal) override;

    /**
     * Generate histogram visualization.
     * @param image Input image
     * @param histogramChannel Channel to visualize histogram for (0 for grayscale)
     * @return Image containing histogram visualization
     */
    cv::Mat histogram(cv::Mat image, int histogramChannel) override;

    /**
     * Perform histogram uniform transformation.
     * @param image Input image to transform
     * @param gMax Maximum output intensity
     * @param gMin Minimum output intensity
     * @return Transformed image with uniform histogram
     */
    cv::Mat histogramUniform(cv::Mat image, int gMax, int gMin) override;

    /**
     * Calculate mean value from histogram.
     * @param imageHistogram Input histogram array
     * @return Mean value
     */
    double mean(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate variance from histogram.
     * @param imageHistogram Input histogram array
     * @return Variance value
     */
    double variance(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate standard deviation from histogram.
     * @param imageHistogram Input histogram array
     * @return Standard deviation value
     */
    double standardDeviation(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate first variation coefficient from histogram.
     * @param imageHistogram Input histogram array
     * @return First variation coefficient
     */
    double variation1(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate asymmetry coefficient from histogram.
     * @param imageHistogram Input histogram array
     * @return Asymmetry coefficient
     */
    double asymmetry(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate flattening coefficient from histogram.
     * @param imageHistogram Input histogram array
     * @return Flattening coefficient
     */
    double flattening(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate second variation coefficient from histogram.
     * @param imageHistogram Input histogram array
     * @return Second variation coefficient
     */
    double variation2(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

    /**
     * Calculate entropy from histogram.
     * @param imageHistogram Input histogram array
     * @return Entropy value
     */
    double entropy(std::array<uint, UCHAR_MAX + 1> imageHistogram) override;

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

    /**
     * Compute inverse Fourier transform.
     * @param fourierImages Vector of Fourier transformed image components
     * @return Reconstructed spatial domain image
     */
    cv::Mat inverseFourierTransform(std::vector<cv::Mat> fourierImages) override;

    /**
     * Compute inverse Fast Fourier transform.
     * @param fourierImages Vector of Fourier transformed image components
     * @return Reconstructed spatial domain image
     */
    cv::Mat inverseFastFourierTransform(std::vector<cv::Mat> fourierImages) override;
};


#endif //GRAYSCALEIMAGEPROCESSOR_H
