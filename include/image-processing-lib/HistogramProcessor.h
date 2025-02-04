//
// Created by gluckasz on 1/31/25.
//

#ifndef HISTOGRAMPROCESSOR_H
#define HISTOGRAMPROCESSOR_H
#include <array>
#include <opencv2/opencv.hpp>


namespace HistogramProcessor {
    /**
     * Compute histogram of the image for a specified channel.
     * @param image Input image
     * @return Array containing histogram values
     */
    [[nodiscard]] std::array<uint, UCHAR_MAX + 1> computeHistogram(cv::Mat image);

    /**
     * Generate histogram visualization.
     * @param channelHistogram Histogram of the input image
     * @param binWidth Width of every bin in a histogram (in pixels)
     * @return Image containing histogram visualization
     */
    cv::Mat histogramVisualization(std::array<uint, UCHAR_MAX + 1> channelHistogram, int binWidth);

    /**
     * Perform histogram equalization.
     * @param image Image to equalize
     * @param imageHistogram Input a histogram array
     * @param gMax Maximum output intensity
     * @param gMin Minimum output intensity
     * @return Transformed image with equalized histogram
     */
    cv::Mat histogramEqualization(const cv::Mat &image, const std::array<uint, UCHAR_MAX + 1> &imageHistogram, int gMax,
                             int gMin);

    /**
     * Calculate mean value from histogram.
     * @param imageHistogram Input a histogram array
     * @return Mean value
     */
    double mean(const std::array<uint, UCHAR_MAX + 1> &imageHistogram);

    /**
     * Calculate variance from histogram.
     * @param imageHistogram Input a histogram array
     * @return Variance value
     */
    double variance(std::array<uint, UCHAR_MAX + 1> imageHistogram);

    /**
     * Calculate standard deviation from histogram.
     * @param imageHistogram Input a histogram array
     * @return Standard deviation value
     */
    double standardDeviation(const std::array<uint, UCHAR_MAX + 1> &imageHistogram);

    /**
     * Calculate the first variation coefficient from histogram.
     * @param imageHistogram Input a histogram array
     * @return First variation coefficient
     */
    double variation1(std::array<uint, UCHAR_MAX + 1> imageHistogram);

    /**
     * Calculate asymmetry coefficient from histogram.
     * @param imageHistogram Input a histogram array
     * @return Asymmetry coefficient
     */
    double asymmetry(std::array<uint, UCHAR_MAX + 1> imageHistogram);

    /**
     * Calculate flattening coefficient from histogram.
     * @param imageHistogram Input a histogram array
     * @return Flattening coefficient
     */
    double flattening(std::array<uint, UCHAR_MAX + 1> imageHistogram);

    /**
     * Calculate the second variation coefficient from histogram.
     * @param imageHistogram Input a histogram array
     * @return Second variation coefficient
     */
    double variation2(const std::array<uint, UCHAR_MAX + 1> &imageHistogram);

    /**
     * Calculate entropy from histogram.
     * @param imageHistogram Input a histogram array
     * @return Entropy value
     */
    double entropy(const std::array<uint, UCHAR_MAX + 1> &imageHistogram);
}


#endif //HISTOGRAMPROCESSOR_H
