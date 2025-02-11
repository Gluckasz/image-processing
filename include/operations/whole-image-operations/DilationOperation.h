//
// Created by gluckasz on 2/4/25.
//
#include "../ImageOperation.h"
#include "image-processing-lib/MorphologicalProcessor.h"

class DilationOperation final : public ImageOperation {
    int maskNum_;

public:
    explicit DilationOperation(const int maskNum) : maskNum_(maskNum) {
    }

    void apply(cv::Mat &image) const override {
        image = MorphologicalProcessor::dilation(image, maskNum_);
    }
};
