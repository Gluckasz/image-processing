//
// Created by gluckasz on 2/4/25.
//
#include "../../include/ImageOperation.h"
#include "../../include/image-processing-lib/MorphologicalProcessor.h"

class ErosionOperation final : public ImageOperation {
    int maskNum_;

public:
    explicit ErosionOperation(const int maskNum) : maskNum_(maskNum) {
    }

    void apply(cv::Mat &image) const override {
        image = MorphologicalProcessor::erosion(image, maskNum_);
    }
};
