//
// Created by gluckasz on 2/3/25.
//
#include "ImageOperation.h"
#include "SpatialDomainProcessor.h"

class NegativeOperation final : public ImageOperation {
public:
    void apply(cv::Mat &image) const override {
        image = SpatialDomainProcessor::negative(image);
    }
};
