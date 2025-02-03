//
// Created by gluckasz on 2/3/25.
//
#include "ImageOperation.h"
#include "SpatialDomainProcessor.h"

class LaplacianFilterOperation final : public ImageOperation {
    int maskNum_;

public:
    explicit LaplacianFilterOperation(const int maskNum) : maskNum_(maskNum) {
    }

    void apply(cv::Mat &image) const override {
        if (image.channels() == 1) {
            image = SpatialDomainProcessor::laplacianFilter<uchar>(image, maskNum_);
        } else {
            image = SpatialDomainProcessor::laplacianFilter<cv::Vec3b>(image, maskNum_);
        }
    }
};
