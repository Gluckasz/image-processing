//
// Created by gluckasz on 2/4/25.
//
#include "../../include/ImageOperation.h"
#include "../../include/MorphologicalProcessor.h"

class OpeningOperation final : public ImageOperation {
    int maskNum_;

public:
    explicit OpeningOperation(const int maskNum) : maskNum_(maskNum) {
    }

    void apply(cv::Mat &image) const override {
        image = MorphologicalProcessor::opening(image, maskNum_);
    }
};
