//
// Created by gluckasz on 2/4/25.
//
#include "../ImageOperation.h"
#include "image-processing-lib/MorphologicalProcessor.h"

class ClosingOperation final : public ImageOperation {
    int maskNum_;

public:
    explicit ClosingOperation(const int maskNum) : maskNum_(maskNum) {
    }

    void apply(cv::Mat &image) const override {
        image = MorphologicalProcessor::closing(image, maskNum_);
    }
};
