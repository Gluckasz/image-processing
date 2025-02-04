//
// Created by gluckasz on 2/3/25.
//
#include "ImageOperation.h"
#include "../../include/input-processing-lib/OutputManager.h"
#include "../../include/image-processing-lib/SpatialDomainProcessor.h"


class RegionGrowingOperation final : public ImageOperation {
    int criterion_;

public:
    explicit RegionGrowingOperation(const int criterion) : criterion_(criterion) {
    }

    void apply(cv::Mat &image) const override {
        cv::Mat segmentedImage;
        cv::Mat segmentedColorImage;
        if (image.channels() == 1) {
            segmentedImage = SpatialDomainProcessor::regionGrowing<uchar>(image, criterion_);
            segmentedColorImage = SpatialDomainProcessor::applyColorMap(
                segmentedImage, SpatialDomainProcessor::createColorMap());
        } else {
            segmentedImage = SpatialDomainProcessor::regionGrowing<cv::Vec3b>(image, criterion_);
            segmentedColorImage = SpatialDomainProcessor::applyColorMap(
                segmentedImage, SpatialDomainProcessor::createColorMap());
        }
        std::string path = OutputManager::constructPath("image", "after_segmentation", "bmp");
        OutputManager::saveImage(segmentedImage, path);
        path = OutputManager::constructPath("image", "after_segmentation_colored", "bmp");
        OutputManager::saveImage(segmentedColorImage, path);
    }
};
