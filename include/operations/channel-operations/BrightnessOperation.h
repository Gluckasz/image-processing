//
// Created by gluckasz on 2/3/25.
//
#include "../ImageOperation.h"
#include "image-processing-lib/SpatialDomainProcessor.h"

class BrightnessOperation final : public ImageOperation {
    int delta_;

public:
    explicit BrightnessOperation(const int delta) : delta_(delta) {
    }

    void apply(cv::Mat &image) const override {
        image = SpatialDomainProcessor::modifyBrightness(image, delta_);
    }
};
