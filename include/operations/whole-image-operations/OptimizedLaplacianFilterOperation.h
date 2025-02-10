//
// Created by gluckasz on 2/3/25.
//
#include "../ImageOperation.h"
#include "image-processing-lib/SpatialDomainProcessor.h"

class OptimizedLaplacianFilterOperation final : public ImageOperation {
public:
    void apply(cv::Mat &image) const override {
        if (image.channels() == 1) {
            image = SpatialDomainProcessor::optimizedLaplacianFilter<uchar>(image);
        } else {
            image = SpatialDomainProcessor::optimizedLaplacianFilter<cv::Vec3b>(image);
        }
    }
};
