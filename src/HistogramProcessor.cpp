//
// Created by gluckasz on 1/31/25.
//

#include "../include/HistogramProcessor.h"

namespace HistogramProcessor {
    std::array<uint, UCHAR_MAX + 1> computeHistogram(cv::Mat image) {
        std::array<uint, UCHAR_MAX + 1> intensityCountArray = {};

        for (int x = 0; x < image.rows; x++) {
            for (int y = 0; y < image.cols; y++) {
                const uint intensity = image.at<uchar>(x, y);
                intensityCountArray[intensity]++;
            }
        }

        return intensityCountArray;
    }

    cv::Mat histogramVisualization(std::array<uint, UCHAR_MAX + 1> channelHistogram,
                                   const int binWidth) {
        const int histogramHeight = static_cast<int>(*std::ranges::max_element(channelHistogram));

        const int histogramWidth = (UCHAR_MAX + 1) * binWidth;
        cv::Mat histogramImage = cv::Mat::zeros(histogramHeight, histogramWidth + binWidth, CV_8UC1);
        for (int y = 0; y < histogramWidth; y++) {
            for (int x = 0; x < channelHistogram[y / binWidth]; x++) {
                histogramImage.at<uchar>(histogramHeight - x - 1, y) = UCHAR_MAX;
            }
        }
        return histogramImage;
    }

    cv::Mat histogramEqualization(const cv::Mat &image,
                             const std::array<uint, UCHAR_MAX + 1> &imageHistogram, const int gMax,
                             const int gMin) {
        cv::Mat result = image.clone();
        const int totalPixels = image.rows * image.cols;
        uint cdf[256] = {static_cast<uint>(imageHistogram[0])};
        for (int i = 1; i < 256; i++)
            cdf[i] = cdf[i - 1] + imageHistogram[i];

        uchar lut[256];
        for (int i = 0; i < 256; i++) {
            lut[i] = std::clamp(
                gMin + static_cast<int>(std::round((gMax - gMin) * (static_cast<double>(cdf[i]) / totalPixels))),
                0,
                UCHAR_MAX
            );
        }

        result.forEach<uchar>([&](uchar &pixel, const int *) {
            pixel = lut[pixel];
        });
        return result;
    }


    double mean(const std::array<uint, UCHAR_MAX + 1> &imageHistogram) {
        double sum = 0;
        double pixels = 0;
        for (int i = 0; i < imageHistogram.size(); i++) {
            sum += imageHistogram[i] * i;
            pixels += imageHistogram[i];
        }

        return sum / pixels;
    }

    double variance(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
        double sum = 0;
        double pixels = 0;
        double meanVal = mean(imageHistogram);
        for (int i = 0; i < imageHistogram.size(); i++) {
            sum += imageHistogram[i] * pow(i - meanVal, 2);
            pixels += imageHistogram[i];
        }

        return sum / pixels;
    }

    double standardDeviation(const std::array<uint, UCHAR_MAX + 1> &imageHistogram) {
        const double varianceVal = variance(imageHistogram);
        return sqrt(varianceVal);
    }

    double variation1(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
        double varianceVal = variance(imageHistogram);
        double meanVal = mean(imageHistogram);
        return varianceVal / meanVal;
    }

    double asymmetry(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
        double sum = 0;
        double pixels = 0;
        double meanVal = mean(imageHistogram);
        for (int i = 0; i < imageHistogram.size(); i++) {
            sum += imageHistogram[i] * pow(i - meanVal, 3);
            pixels += imageHistogram[i];
        }

        double standardDeviationVal = standardDeviation(imageHistogram);

        return sum / pixels / pow(standardDeviationVal, 3);
    }

    double flattening(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
        double sum = 0;
        double pixels = 0;
        double meanVal = mean(imageHistogram);
        for (int i = 0; i < imageHistogram.size(); i++) {
            sum += imageHistogram[i] * pow(i - meanVal, 4);
            pixels += imageHistogram[i];
        }

        double varianceVal = variance(imageHistogram);

        return sum / pixels / pow(varianceVal, 2) - 3;
    }

    double variation2(const std::array<uint, UCHAR_MAX + 1> &imageHistogram) {
        double sum = 0;
        double pixels = 0;
        for (const unsigned int i: imageHistogram) {
            sum += pow(i, 2);
            pixels += i;
        }

        return sum / pow(pixels, 2);
    }

    double entropy(const std::array<uint, UCHAR_MAX + 1> &imageHistogram) {
        double sum = 0;
        double pixels = 0;
        for (const unsigned int i: imageHistogram) {
            pixels += i;
        }

        for (const unsigned int i: imageHistogram) {
            if (i > 0) {
                sum += i * log2(i / pixels);
            }
        }

        return -sum / pixels;
    }
}
