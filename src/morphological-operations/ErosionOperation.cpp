//
// Created by gluckasz on 2/4/25.
//
#include "ImageOperation.h"
#include "MorphologicalProcessor.h"

class LaplacianFilterOperation final : public ImageOperation {
    int maskNum_;

public:
    explicit LaplacianFilterOperation(const int maskNum) : maskNum_(maskNum) {
    }

    void apply(cv::Mat &image) const override {
        image = MorphologicalProcessor::erosion(image, maskNum_);
    }
};
