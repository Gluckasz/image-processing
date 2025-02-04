//
// Created by gluckasz on 2/4/25.
//
#include "ImageComparer.h"
#include "ImageOperation.h"
#include "OutputManager.h"

class CompareImageStatsOperation final : public ImageOperation {
    cv::Mat &originalImage_;

public:
    explicit CompareImageStatsOperation(cv::Mat &originalImage) : originalImage_(originalImage) {
    }

    void apply(cv::Mat &newImage) const override {
        std::vector<cv::Mat> originalImageChannels, newImageChannels;
        split(originalImage_, originalImageChannels);
        split(newImage, newImageChannels);
        double meanSquareErrorValue = 0, peakMeanSquareErrorValue = 0, signalToNoiseRatioValue = 0,
                peakSignalToNoiseRatioValue = 0, maximumDifferenceValue = 0;
        for (int i = 0; i < newImageChannels.size(); i++) {
            meanSquareErrorValue += ImageComparer::meanSquareError(originalImageChannels[i], newImageChannels[i]);
            peakMeanSquareErrorValue += ImageComparer::peakMeanSquareError(
                originalImageChannels[i], newImageChannels[i]);
            signalToNoiseRatioValue += ImageComparer::signalToNoiseRatio(originalImageChannels[i], newImageChannels[i]);
            peakSignalToNoiseRatioValue += ImageComparer::peakSignalToNoiseRatio(
                originalImageChannels[i], newImageChannels[i]);
            maximumDifferenceValue += ImageComparer::maximumDifference(originalImageChannels[i], newImageChannels[i]);
        }
        std::stringstream ss;
        ss << "Comparison image stats:\n";
        ss << "Mean square error: " << meanSquareErrorValue / static_cast<double>(newImageChannels.size()) << "\n"
                << "Peak mean square error: " << peakMeanSquareErrorValue / static_cast<double>(newImageChannels.size())
                << "\n"
                << "Signal to noise ratio: " << signalToNoiseRatioValue / static_cast<double>(newImageChannels.size())
                << "\n"
                << "Peak signal to noise ratio: " << peakSignalToNoiseRatioValue / static_cast<double>(newImageChannels.
                    size()) << "\n"
                << "Maximum difference: " << maximumDifferenceValue / static_cast<double>(newImageChannels.size()) <<
                "\n";
        const std::string path = OutputManager::constructPath("image", "comparison_stats", "txt");
        OutputManager::saveFile(path, ss);
    }
};
