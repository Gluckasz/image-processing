//
// Created by gluckasz on 2/3/25.
//
#include "ImageOperation.h"
#include "../../include/image-processing-lib/SpatialDomainProcessor.h"

class FlipDiagonallyOperation final : public ImageOperation {
public:
    void apply(cv::Mat &image) const override {
        image = SpatialDomainProcessor::flipDiagonally(image);
    }
};
