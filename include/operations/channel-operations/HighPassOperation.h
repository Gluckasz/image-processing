//
// Created by gluckasz on 2/4/25.
//
#include "../ImageOperation.h"
#include "input-processing-lib/OutputManager.h"
#include "image-processing-lib/FourierProcessor.h"

class HighPassOperation final : public ImageOperation {
    int maskSize_;

public:
    explicit HighPassOperation(const int maskSize) : maskSize_(maskSize) {
    }

    void apply(cv::Mat &image) const override {
        cv::Mat fourierImage = FourierProcessor::fastFourierTransform(image);
        fourierImage = FourierProcessor::fftHighPass(fourierImage, maskSize_);
        const std::string path = OutputManager::constructPath("image_fourier", "magnitude_spectrum", "bmp");
        FourierProcessor::visualizeFourier(fourierImage, path);
        image = FourierProcessor::inverseFastFourierTransform(fourierImage);
    }
};

