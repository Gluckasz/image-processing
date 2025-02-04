//
// Created by gluckasz on 2/4/25.
//
#include "../Constants.h"
#include "../image-processing-lib/HistogramProcessor.h"
#include "../ImageOperation.h"
#include "../input-processing-lib/OutputManager.h"

class HistogramVisualizationOperation final : public ImageOperation {
    int histogramChannel_;

public:
    explicit HistogramVisualizationOperation(const int histogramChannel) : histogramChannel_(histogramChannel) {
    }
    void apply(cv::Mat &image) const override {
        std::vector<cv::Mat> channels;
        split(image, channels);
        std::array<uint, UCHAR_MAX + 1> channelHistogram = HistogramProcessor::computeHistogram(channels[histogramChannel_]);
        cv::Mat histogramImage = HistogramProcessor::histogramVisualization(channelHistogram, Constants::DEFAULT_BIN_WIDTH);
        std::string path = OutputManager::constructPath("image", "histogram", "png");
        OutputManager::saveImage(histogramImage, path);
    }
};
