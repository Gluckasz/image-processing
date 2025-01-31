//
// Created by gluckasz on 1/31/25.
//

#include "../include/HistogramProcessor.h"

template<typename PixelType>
std::array<int, UCHAR_MAX + 1> HistogramProcessor::computeHistogram(cv::Mat image, int histogramChannel) {
    std::array<int, UCHAR_MAX + 1> intensityCountArray = {};

    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            PixelType pixel = image.at<PixelType>(x, y);
            int intensity;
            if constexpr (std::is_same_v<PixelType, cv::Vec3b>) {
                intensity = pixel[histogramChannel];
            } else {
                intensity = pixel;
            }

            intensityCountArray[intensity]++;
        }
    }

    return intensityCountArray;
}

cv::Mat HistogramProcessor::histogramVisualization(std::array<int, UCHAR_MAX + 1> channelHistogram, int binWidth) {
    const int histogramHeight = *std::ranges::max_element(channelHistogram);

    const int histogramWidth = (UCHAR_MAX + 1) * binWidth;
    cv::Mat histogramImage = cv::Mat::zeros(histogramHeight, histogramWidth + binWidth, CV_8UC1);
    for (int y = 0; y < histogramWidth; y++) {
        for (int x = 0; x < channelHistogram[y / binWidth]; x++) {
            histogramImage.at<uchar>(histogramHeight - x - 1, y) = UCHAR_MAX;
        }
    }
    return histogramImage;
}

template<typename PixelType>
cv::Mat HistogramProcessor::histogramUniform(const cv::Mat& image, int gMax, int gMin) {
    cv::Mat result = image.clone();
    const int totalPixels = image.rows * image.cols;

    if constexpr (std::is_same_v<PixelType, uchar>) {
        std::array<int, UCHAR_MAX + 1> imageHistogram = computeHistogram<PixelType>(image, 0);

        uint cdf[256] = {imageHistogram[0]};
        for (int i = 1; i < 256; i++)
            cdf[i] = cdf[i-1] + imageHistogram[i];

        uchar lut[256];
        for (int i = 0; i < 256; i++) {
            lut[i] = std::clamp(
                gMin + static_cast<int>(std::round((gMax - gMin) * (static_cast<double>(cdf[i]) / totalPixels))),
                0,
                UCHAR_MAX
                );
        }

        result.forEach<uchar>([&](uchar& pixel, const int*) {
            pixel = lut[pixel];
        });
    }
    else if constexpr (std::is_same_v<PixelType, cv::Vec3b>) {
        std::vector<cv::Mat> channels;
        split(image, channels);

        for (auto& channel : channels) {
            std::array<int, UCHAR_MAX + 1> imageHistogram = computeHistogram<PixelType>(image, 0);

            uint cdf[256] = {imageHistogram[0]};
            for (int i = 1; i < 256; i++)
                cdf[i] = cdf[i-1] + imageHistogram[i];

            uchar lut[256];
            for (int i = 0; i < 256; i++) {
                lut[i] = std::clamp(
                gMin + static_cast<int>(std::round((gMax - gMin) * (static_cast<double>(cdf[i]) / totalPixels))),
                0,
                UCHAR_MAX
                );
            }

            channel.forEach<uchar>([&](uchar& pixel, const int*) {
                pixel = lut[pixel];
            });
        }

        cv::merge(channels, result);
    }

    return result;
}


double HistogramProcessor::mean(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * i;
        pixels += imageHistogram[i];
    }

    return sum / pixels;
}

double HistogramProcessor::variance(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    double mean = this->mean(imageHistogram);
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * pow(i - mean, 2);
        pixels += imageHistogram[i];
    }

    return sum / pixels;
}

double HistogramProcessor::standardDeviation(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double variance = this->variance(imageHistogram);
    return sqrt(variance);
}

double HistogramProcessor::variation1(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double variance = this->variance(imageHistogram);
    double mean = this->mean(imageHistogram);
    return variance / mean;
}

double HistogramProcessor::asymmetry(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    double mean = this->mean(imageHistogram);
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * pow(i - mean, 3);
        pixels += imageHistogram[i];
    }

    double standardDeviation = this->standardDeviation(imageHistogram);

    return sum / pixels / pow(standardDeviation, 3);
}

double HistogramProcessor::flattening(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    double mean = this->mean(imageHistogram);
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * pow(i - mean, 4);
        pixels += imageHistogram[i];
    }

    double variance = this->variance(imageHistogram);

    return sum / pixels / pow(variance, 2)  - 3;
}

double HistogramProcessor::variation2(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    for (unsigned int i : imageHistogram) {
        sum += pow(i, 2);
        pixels += i;
    }

    return sum / pow(pixels, 2);
}

double HistogramProcessor::entropy(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    for (unsigned int i : imageHistogram) {
        pixels += i;
    }

    for (unsigned int i : imageHistogram) {
        if (i > 0) {
            sum += i * log2(i / pixels);
        }
    }

    return - sum / pixels;
}