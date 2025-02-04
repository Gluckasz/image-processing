//
// Created by gluckasz on 2/3/25.
//
#include "ImageOperation.h"
#include "../../include/image-processing-lib/SpatialDomainProcessor.h"

class MidpointFilterOperation final : public ImageOperation {
    int kernelSize_;

public:
    explicit MidpointFilterOperation(const int kernelSize) : kernelSize_(kernelSize) {
    }

    void apply(cv::Mat &image) const override {
        image = SpatialDomainProcessor::midpointFilter(image, kernelSize_);
    }
};
