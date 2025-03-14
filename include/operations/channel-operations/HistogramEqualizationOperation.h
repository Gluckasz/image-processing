//
// Created by gluckasz on 2/4/25.
//
#include "image-processing-lib/HistogramProcessor.h"
#include "../ImageOperation.h"

class HistogramEqualizationOperation final : public ImageOperation {
    int gMin_, gMax_;
public:
    explicit HistogramEqualizationOperation(const int gMin, const int gMax) : gMin_(gMin), gMax_(gMax) {
    }

    void apply(cv::Mat &image) const override {
        std::array<uint, UCHAR_MAX + 1> channelHistogram = HistogramProcessor::computeHistogram(image);
        image = HistogramProcessor::histogramEqualization(image, channelHistogram, gMax_, gMin_);
    }
};
