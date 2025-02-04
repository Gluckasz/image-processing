//
// Created by gluckasz on 2/3/25.
//
#include "ImageOperation.h"
#include "../../include/image-processing-lib/SpatialDomainProcessor.h"

class RobertsOperation final : public ImageOperation {
public:
    void apply(cv::Mat &image) const override {
        if (image.channels() == 1) {
            image = SpatialDomainProcessor::robertsOperator1<uchar>(image);
        } else {
            image = SpatialDomainProcessor::robertsOperator1<cv::Vec3b>(image);
        }
    }
};
