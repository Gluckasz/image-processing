//
// Created by gluckasz on 2/4/25.
//
#include "../../include/Constants.h"
#include "../../include/HistogramProcessor.h"
#include "../../include/ImageOperation.h"
#include "../../include/OutputManager.h"

class HistogramVisualizationOperation final : public ImageOperation {
public:
    void apply(cv::Mat &image) const override {
        std::array<uint, UCHAR_MAX + 1> channelHistogram = HistogramProcessor::computeHistogram(image);
        cv::Mat histogramImage = HistogramProcessor::histogramVisualization(channelHistogram, Constants::DEFAULT_BIN_WIDTH);
        std::string path = OutputManager::constructPath("image", "histogram", "png");
        OutputManager::saveImage(histogramImage, path);
    }
};
