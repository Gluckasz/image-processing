//
// Created by gluckasz on 2/3/25.
//
#include "ImageOperation.h"
#include "SpatialDomainProcessor.h"

class ContrastGammaOperation final : public ImageOperation {
    float delta_;

public:
    explicit ContrastGammaOperation(const float delta) : delta_(delta) {
    }

    void apply(cv::Mat &image) const override {
        image = SpatialDomainProcessor::modifyContrastGamma(image, delta_);
    }
};
