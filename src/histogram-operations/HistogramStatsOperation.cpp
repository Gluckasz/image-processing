//
// Created by gluckasz on 2/4/25.
//
#include "HistogramProcessor.h"
#include "ImageOperation.h"
#include "OutputManager.h"

class HistogramStatsOperation final : public ImageOperation {
public:
    void apply(cv::Mat &image) const override {
        std::vector<cv::Mat> imageChannels;
        split(image, imageChannels);
        double meanValue = 0, varianceValue = 0, standardDeviationValue = 0, variation1Value = 0, asymmetryValue = 0,
                flatteningValue = 0, variation2Value = 0, entropyValue = 0;
        for (int i = 0; i < imageChannels.size(); i++) {
            std::array<uint, UCHAR_MAX + 1> channelHistogram = HistogramProcessor::computeHistogram(image);
            meanValue += HistogramProcessor::mean(channelHistogram);
            varianceValue += HistogramProcessor::variance(channelHistogram);
            standardDeviationValue += HistogramProcessor::standardDeviation(channelHistogram);
            variation1Value += HistogramProcessor::variation1(channelHistogram);
            asymmetryValue += HistogramProcessor::asymmetry(channelHistogram);
            flatteningValue += HistogramProcessor::flattening(channelHistogram);
            variation2Value += HistogramProcessor::variation2(channelHistogram);
            entropyValue += HistogramProcessor::entropy(channelHistogram);
        }
        std::stringstream ss;
        ss << "Image stats (computed from histogram):\n";
        ss << "Mean: " << meanValue / static_cast<double>(imageChannels.size()) << "\n"
                << "Variance: " << varianceValue / static_cast<double>(imageChannels.size()) << "\n"
                << "Standard deviation: " << standardDeviationValue / static_cast<double>(imageChannels.size()) << "\n"
                << "Variation 1 coefficient: " << variation1Value / static_cast<double>(imageChannels.size()) << "\n"
                << "Asymmetry coefficient: " << asymmetryValue / static_cast<double>(imageChannels.size()) << "\n"
                << "Flattening coefficient: " << flatteningValue / static_cast<double>(imageChannels.size()) << "\n"
                << "Variation 2 coefficient: " << variation2Value / static_cast<double>(imageChannels.size()) << "\n"
                << "Entropy: " << entropyValue / static_cast<double>(imageChannels.size()) << "\n";
        const std::string path = OutputManager::constructPath("image", "histogram_stats", "txt");
        OutputManager::saveFile(path, ss);
    }
};
