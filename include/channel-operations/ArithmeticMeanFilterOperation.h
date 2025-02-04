//
// Created by gluckasz on 2/3/25.
//
#include "ImageOperation.h"
#include "../../include/image-processing-lib/SpatialDomainProcessor.h"

class ArithmeticMeanFilterOperation final : public ImageOperation {
    int kernelSize_;

public:
    explicit ArithmeticMeanFilterOperation(const int kernelSize) : kernelSize_(kernelSize) {
    }

    void apply(cv::Mat &image) const override {
        image = SpatialDomainProcessor::arithmeticMeanFilter(image, kernelSize_);
    }
};
