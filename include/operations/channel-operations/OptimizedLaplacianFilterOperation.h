//
// Created by gluckasz on 2/3/25.
//
#include "../ImageOperation.h"
#include "../../image-processing-lib/SpatialDomainProcessor.h"

class OptimizedLaplacianFilterOperation final : public ImageOperation {
public:
    void apply(cv::Mat &image) const override {
        image = SpatialDomainProcessor::optimizedLaplacianFilter(image);
    }
};
