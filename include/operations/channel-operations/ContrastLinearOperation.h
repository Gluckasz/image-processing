//
// Created by gluckasz on 2/3/25.
//
#include "../ImageOperation.h"
#include "image-processing-lib/SpatialDomainProcessor.h"

class ContrastLinearOperation final : public ImageOperation {
    int delta_;

public:
    explicit ContrastLinearOperation(const int delta) : delta_(delta) {
    }

    void apply(cv::Mat &image) const override {
        image = SpatialDomainProcessor::modifyContrastLinear(image, delta_);
    }
};
