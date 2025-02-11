//
// Created by gluckasz on 2/4/25.
//
#include "../ImageOperation.h"
#include "image-processing-lib/MorphologicalProcessor.h"

class HMTOperation final : public ImageOperation {
    int maskNum_;

public:
    explicit HMTOperation(const int maskNum) : maskNum_(maskNum) {
    }

    void apply(cv::Mat &image) const override {
        image = MorphologicalProcessor::hmt(image, maskNum_);
    }
};
