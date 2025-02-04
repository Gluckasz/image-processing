//
// Created by gluckasz on 2/4/25.
//
#include "../../include/FourierProcessor.h"
#include "../../include/ImageOperation.h"
#include "../../include/input-processing-lib/OutputManager.h"

class FastFourierOperation final : public ImageOperation {
public:
    void apply(cv::Mat &image) const override {
        cv::Mat fourierImage = FourierProcessor::fastFourierTransform(image);
        const std::string path = OutputManager::constructPath("image_fourier", "magnitude_spectrum", "bmp");
        FourierProcessor::visualizeFourier(image, path);
        image = FourierProcessor::inverseFastFourierTransform(fourierImage);
    }
};

