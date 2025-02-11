//
// Created by gluckasz on 2/3/25.
//
#include "../ImageOperation.h"
#include "../../image-processing-lib/SpatialDomainProcessor.h"

class LaplacianFilterOperation final : public ImageOperation {
    int maskNum_;

public:
    explicit LaplacianFilterOperation(const int maskNum) : maskNum_(maskNum) {
    }

    void apply(cv::Mat &image) const override {
        image = SpatialDomainProcessor::laplacianFilter(image, maskNum_);
    }
};
