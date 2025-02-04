//
// Created by gluckasz on 2/4/25.
//
#include "../../include/ImageOperation.h"
#include "../../include/input-processing-lib/OutputManager.h"
#include "image-processing-lib/FourierProcessor.h"

class LowPassOperation final : public ImageOperation {
    int maskSize_;

public:
    explicit LowPassOperation(const int maskSize) : maskSize_(maskSize) {
    }

    void apply(cv::Mat &image) const override {
        cv::Mat fourierImage = FourierProcessor::fastFourierTransform(image);
        fourierImage = FourierProcessor::fftLowPass(fourierImage, maskSize_);
        const std::string path = OutputManager::constructPath("image_fourier", "magnitude_spectrum", "bmp");
        FourierProcessor::visualizeFourier(fourierImage, path);
        image = FourierProcessor::inverseFastFourierTransform(fourierImage);
    }
};

